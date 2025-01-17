#!/usr/bin/ruby

#------------------------------------------------------------------------------
# build-update-kit-win32.rb
#------------------------------------------------------------------------------
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301  USA
#
#------------------------------------------------------------------------------
# Copyright (C) 2013 - 2015 "Zalewa" <zalewapl@gmail.com>
#------------------------------------------------------------------------------

# Instructions of use:
#
# This script is a part of Doomseeker update packages auto-build system.
#
# What this does:
# 1. For every package that is returned by program when --version-json arg
#    is used, there is a call to CREATE_PACKAGE_SCRIPT_NAME script.
#    Platform, display version and suffix information is passed to this script
#    as arguments.
# 2. An output directory is determined basing on current timestamp and randomly
#    generated string of 8 characters. This directory is created in current
#    working directory.
# 3. CREATE_PACKAGE_SCRIPT_NAME script creates two files in the output
#    directory. One file is a package .zip archive, the other is a Mendeley
#    updater's XML script. Files are named basing on internal package name,
#    human-readable version, revision, channel and platform.
# 4. If at least one package was created successfully, this script will
#    create an "update-info.js" file. This file contains information on
#    all packages for current channel. Doomseeker downloads this file each time
#    when it wants to check if there are any new updates. OVERWRITE THIS FILE
#    ON THE SERVER WITH ABSOLUTE CARE!!! The file is formatted in
#    a human-readable fashion for purpose. Always double-check if revisions for
#    all channels are correct, otherwise you may screw up people's setups.
#
# Requirements:
# Following requirements must be met for this script to work correctly:
# - "ruby" executable must be in ENV["PATH"].
# - 'os' gem must be installed:
#   `gem install os`
# - CREATE_PACKAGE_SCRIPT_NAME must exist in current working directory and its
#   requirements must be met.
# - PACKAGE_CONFIGS_DIR must exist and contain .js files, one for each package.
#   .js files must be compliant with config-template.js. Files must be named
#   after internal package names (doomseeker-core.js, p-chocolatedoom.js, etc.).
# - Doomseeker binary packages need to be compiled beforehand and placed in
#   a directory in a structure that is the same as after deployment on end-user
#   system. All Doomseeker requirements and peripheral files must also be
#   located in this directory (translations, plugins, runtime DLLs, etc.).
#   `make install`, or INSTALL target in Visual Studio, should take care of
#   creating appropriate structure.
#
# Usage from build script:
#   Just run 'package_beta' or 'package_stable' targets. It'll take care of
#   prior building and installation and spawn update packages in
#   ${CMAKE_BUILD_DIR}. The exact, full, absolute path is always printed
#   to stderr (console output) so check there if you can't find your package.
#
# Usage:
#    Call pattern & arguments:
#      script.rb -c <channel> -i <binary-dir>
#
#    -c <channel>
#        Update channel for which the packages will be generated. It must be
#        either "stable" or "beta". This is case-sensitive.
#    -i <binary-dir>
#        Directory where doomseeker.exe resides.

require 'fileutils'
require 'optparse'
require 'rubygems'
require 'json'
require 'securerandom'
require 'date'
require 'tempfile'
require 'OS'

###############################################################################
# Configuration Data
###############################################################################
if OS.windows?
    PACKAGE_PLATFORM = "win32"
    PACKAGE_CONFIGS_DIR = "win32-configs"
    MAIN_EXECUTABLE_FILENAME = "doomseeker.exe"
elsif OS.mac?
    PACKAGE_PLATFORM = "macosx"
    PACKAGE_CONFIGS_DIR = "macosx-configs"
    MAIN_EXECUTABLE_FILENAME = "Contents/MacOS/doomseeker"
end
CREATE_PACKAGE_SCRIPT_NAME = "create-packages.rb"
URL_BASE = "https://doomseeker.drdteam.org/updates/"


###############################################################################
# Functions
###############################################################################
def spawn_unique_dir(prefix)
    # There's a slight possibility that the directory will already exist,
    # however with random and time stamp the risk should be minimal.
    stamp = DateTime.now.strftime("%Y%m%d-%H%M%S")
    # Make sure only letters and digits are allowed in 'random'. This shall
    # prevent generating "difficult" paths.
    random = SecureRandom.base64(8).gsub(/[^[:alnum:]]/, "_")
    name = "#{prefix}-#{stamp}-#{random}"
    raise "Directory #{name} already exists." if File.directory?(name)
    Dir.mkdir(name)
    return name
end

def obtain_version_information(binary_dir)
    file = Tempfile.new('program-version.json')
    begin
        result = system(File.join(binary_dir, MAIN_EXECUTABLE_FILENAME),
            "--version-json", file.path)
        raise "Failed to obtain version information" if !result
        file.rewind
        jsondata = file.read
        versiondata = JSON.parse(jsondata)
        return filter_blacklisted_packages(versiondata)
    ensure
        file.close
        file.unlink
    end
end

def package_suffix(display_version, revision, channel, platform)
    suffix = "#{revision}-#{channel}_#{platform}"
    if display_version != revision
        suffix = "#{display_version}_#{suffix}"
    end
    return "_#{suffix}"
end

def package_filename(pkg_name, display_version, revision, channel, platform)
    return "#{pkg_name}#{package_suffix(display_version, revision, channel, platform)}.zip"
end

def extract_display_version(pkg_data)
    return pkg_data.include?("display-version") ?
        pkg_data["display-version"] : pkg_data["revision"].to_s
end

def process_package(pkg_name, channel, version_data, binary_dir, output_dir)
    # Create the package archive and .xml script by calling
    # the "create-packages" script.

    # Extract necessary information on the package.
    revision = version_data["revision"]
    display_version = extract_display_version(version_data)
    suffix = package_suffix(display_version, revision, channel, PACKAGE_PLATFORM)
    # Get path to the .js config file required by the script.
    cfg_file_path = File.join(PACKAGE_CONFIGS_DIR, "#{pkg_name}.js")
    # Run script.
    scriptdir = File.dirname(__FILE__)
    result = system("ruby", "#{scriptdir}/#{CREATE_PACKAGE_SCRIPT_NAME}",
        "-p", PACKAGE_PLATFORM, "-v", display_version, "--suffix", suffix,
        binary_dir, "#{scriptdir}/#{cfg_file_path}", output_dir)
    raise "Package generation failed." if !result
end

def dump_update_info(output_path, channel, version_data)
    # Convert internal channel information to data understood by Doomseeker's
    # update-info parser.
    update_info = {}
    version_data.each do |pkg, pkg_info|
        revision = pkg_info["revision"]
        display_version = extract_display_version(pkg_info)
        filename = package_filename(pkg, display_version, revision, channel, PACKAGE_PLATFORM)
        url = File.join(URL_BASE, PACKAGE_PLATFORM, channel, filename)
        update_info[pkg] = {
            "revision" => revision,
            "display-version" => extract_display_version(pkg_info),
            "display-name" => pkg_info["display-name"],
            "URL" => url
        }
    end
    json_data = JSON.pretty_generate(update_info)
    File.open(output_path, "w") do |f|
        f.write(json_data)
        f.write("\n")
    end
end

def filter_blacklisted_packages(versions)
    # TODO: blacklist doomseeker for 'beta' update channel,
    # then hardcode a final doomseeker.zip package in the update-info file.
    # This final doomseeker.zip should already be aware of split
    # packages (qt.zip, wadseeker.zip, doomseeker-core.zip).
    # And don't forget that Mac and Windows can be different here.
    return versions
end
###############################################################################
# Script Contents
###############################################################################

# Parse arguments.
target_channel = nil
binary_dir = nil

OptionParser.new do |parser|
    parser.banner = "#{$0} -c <channel> -i <binary_dir>"
    parser.on("-c", "--channel <update channel>",
            "Specifies the update channel for which the update " \
                "packages will be generated. Must be either \"beta\" "\
                "or \"stable\".") do |val|
        target_channel = val
    end
    parser.on("-i", "--input [binary_dir]",
        "Directory with deployed Doomseeker, from which packages " \
            "are created.") do |val|
        binary_dir = val
    end
end.parse!

# Validate and process arguments.
begin
    raise "Channel not specified (use -c option)" if !target_channel
    raise "Binary directory not specified (use -i option)" if !binary_dir
    raise "Binary directory \"#{binary_dir}\" doesn't exist." \
        if !File.directory?(binary_dir)
    target_channel.downcase!
rescue
    $stderr.puts "ERROR: #{$!}"
    $stderr.puts "Use -h for help"
    exit(4)
end

versions = obtain_version_information(binary_dir)
output_dir = spawn_unique_dir("upkgs-#{target_channel}")
packages_output_dir = File.join(output_dir, PACKAGE_PLATFORM, target_channel)

# Process packages.
successes = []
failures = []
versions.each do |pkg_name, version_data|
    if pkg_name == "doomseeker"
        $stderr.puts "==== Skipping: #{pkg_name}"
        next
    end
    begin
        $stderr.puts "==== Now processing: #{pkg_name}"
        process_package(pkg_name, target_channel, version_data, binary_dir, packages_output_dir)
        successes << pkg_name
    rescue
        puts $@, $!
        failures << {
            "name" => pkg_name,
            "error" => $!
        }
    end
end

# Print success/failure states.
successes.each do |el|
    $stderr.puts "Success: #{el}"
end

failures.each do |el|
    $stderr.puts "FAILURE: #{el["name"]}: #{el["error"].message}"
end

if !successes.empty?
    $stderr.puts "Created packages are in directory: #{output_dir}"
    fullpath = File.expand_path(output_dir)
    $stderr.puts "#{fullpath}"
    # If at least one package was successful create the update-info.js file.
    update_info_path = File.join(output_dir, "update-info_#{PACKAGE_PLATFORM}_#{target_channel}.js")
    $stderr.puts "Creating update info file: #{update_info_path}"
    dump_update_info(update_info_path, target_channel, versions)
else
    # Delete the output directory if all packages failed.
    begin
        Dir.rmdir(output_dir)
    rescue
        puts $@, $!
    end
end

$stderr.puts failures.empty? ? "ALL SUCCESS!" : "ERRORS ENCOUNTERED!"
exit(failures.empty? ? 0 : 3)
