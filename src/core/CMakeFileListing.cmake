set(UI_FILES
	gui/aboutdialog.ui
	gui/addBuddyDlg.ui
	gui/configuration/cfgappearance.ui
	gui/configuration/cfgautoupdates.ui
	gui/configuration/cfgcustomservers.ui
	gui/configuration/cfgfilepaths.ui
	gui/configuration/cfgquery.ui
	gui/configuration/cfgserverpasswords.ui
	gui/configuration/cfgwadalias.ui
	gui/configuration/cfgwadseekerappearance.ui
	gui/configuration/cfgwadseekergeneral.ui
	gui/configuration/cfgwadseekeridgames.ui
	gui/configuration/cfgwadseekersites.ui
	gui/configuration/configurationdialog.ui
	gui/configuration/engineconfigpage.ui
	gui/configuration/irc/cfgchatlogspage.ui
	gui/configuration/irc/cfgircappearance.ui
	gui/configuration/irc/cfgircdefinenetworkdialog.ui
	gui/configuration/irc/cfgircnetworks.ui
	gui/configuration/irc/cfgircsounds.ui
	gui/copytextdlg.ui
	gui/createserver/customparamspanel.ui
	gui/createserver/dmflagspanel.ui
	gui/createserver/gameexecutablepicker.ui
	gui/createserver/gamerulespanel.ui
	gui/createserver/generalgamesetuppanel.ui
	gui/createserver/iwadandwadspickerdialog.ui
	gui/createserver/iwadpicker.ui
	gui/createserver/maplistpanel.ui
	gui/createserver/miscserversetuppanel.ui
	gui/createserver/wadspicker.ui
	gui/createserverdialog.ui
	gui/demomanager.ui
	gui/dockBuddiesList.ui
	gui/freedoomdialog.ui
	gui/ip2cupdatebox.ui
	gui/irc/ircdock.ui
	gui/irc/ircdocktabcontents.ui
	gui/irc/ircignoresmanager.ui
	gui/irc/ircnetworkselectionbox.ui
	gui/logdock.ui
	gui/mainwindow.ui
	gui/missingwadsdialog.ui
	gui/passworddlg.ui
	gui/programargshelpdialog.ui
	gui/rconpassworddialog.ui
	gui/remoteconsole.ui
	gui/serverdetailsdock.ui
	gui/serverfilterdock.ui
	gui/wadseekerinterface.ui
	gui/widgets/filepickwidget.ui
	gui/widgets/importantmessageswidget.ui
	gui/widgets/serverconsole.ui
)
set(HEADER_FILES
	application.h
	apprunner.h
	clipboard.h
	cmdargshelp.h
	commandline.h
	commandlinetokenizer.h
	configuration/doomseekerconfig.h
	configuration/passwordscfg.h
	configuration/queryspeed.h
	configuration/serverpassword.h
	configuration/serverpasswordsummary.h
	connectionhandler.h
	customservers.h
	datapaths.h
	datastreamoperatorwrapper.h
	doomseekerfilepaths.h
	dptr.h
	filefilter.h
	fileutils.h
	gamedemo.h
	global.h
	gui/aboutdialog.h
	gui/commongui.h
	gui/configuration/cfgappearance.h
	gui/configuration/cfgautoupdates.h
	gui/configuration/cfgcustomservers.h
	gui/configuration/cfgfilepaths.h
	gui/configuration/cfgquery.h
	gui/configuration/cfgserverpasswords.h
	gui/configuration/cfgwadalias.h
	gui/configuration/cfgwadseekerappearance.h
	gui/configuration/cfgwadseekergeneral.h
	gui/configuration/cfgwadseekeridgames.h
	gui/configuration/cfgwadseekersites.h
	gui/configuration/configpage.h
	gui/configuration/configurationdialog.h
	gui/configuration/doomseekerconfigurationdialog.h
	gui/configuration/engineconfigpage.h
	gui/configuration/irc/cfgchatlogspage.h
	gui/configuration/irc/cfgircappearance.h
	gui/configuration/irc/cfgircdefinenetworkdialog.h
	gui/configuration/irc/cfgircnetworks.h
	gui/configuration/irc/cfgircsounds.h
	gui/configuration/irc/ircconfigurationdialog.h
	gui/copytextdlg.h
	gui/createserver/customparamspanel.h
	gui/createserver/dmflagspanel.h
	gui/createserver/gameexecutablepicker.h
	gui/createserver/gamerulespanel.h
	gui/createserver/generalgamesetuppanel.h
	gui/createserver/iwadandwadspickerdialog.h
	gui/createserver/iwadpicker.h
	gui/createserver/maplistpanel.h
	gui/createserver/miscserversetuppanel.h
	gui/createserver/wadspicker.h
	gui/createserverdialog.h
	gui/demomanager.h
	gui/dockBuddiesList.h
	gui/entity/serverlistfilterinfo.h
	gui/entity/showmode.h
	gui/freedoomdialog.h
	gui/helpers/comboboxex.h
	gui/helpers/datetablewidgetitem.h
	gui/helpers/playersdiagram.h
	gui/helpers/taskbarbutton.h
	gui/helpers/taskbarprogress.h
	gui/ip2cupdatebox.h
	gui/irc/ircdock.h
	gui/irc/ircdocktabcontents.h
	gui/irc/ircignoresmanager.h
	gui/irc/ircnetworkselectionbox.h
	gui/irc/ircsounds.h
	gui/irc/ircuserlistmodel.h
	gui/logdock.h
	gui/mainwindow.h
	gui/missingwadsdialog.h
	gui/models/serverlistcolumn.h
	gui/models/serverlistmodel.h
	gui/models/serverlistproxymodel.h
	gui/models/serverlistrowhandler.h
	gui/passworddlg.h
	gui/programargshelpdialog.h
	gui/rconpassworddialog.h
	gui/remoteconsole.h
	gui/serverdetailsdock.h
	gui/serverfilterdock.h
	gui/serverlist.h
	gui/standardserverconsole.h
	gui/wadseekerinterface.h
	gui/wadseekershow.h
	gui/widgets/colorbutton.h
	gui/widgets/createserverdialogpage.h
	gui/widgets/dndfilesystemlistview.h
	gui/widgets/engineplugincombobox.h
	gui/widgets/filepickwidget.h
	gui/widgets/fontbutton.h
	gui/widgets/iconlabel.h
	gui/widgets/importantmessageswidget.h
	gui/widgets/memorylineedit.h
	gui/widgets/multicombobox.h
	gui/widgets/qtabwidgetext.h
	gui/widgets/serverconsole.h
	gui/widgets/serverfilterbuildermenu.h
	gui/widgets/serverlistcontextmenu.h
	gui/widgets/serverlistview.h
	gui/widgets/serversstatuswidget.h
	gui/widgets/tablewidgetmouseaware.h
	gui/widgets/wadseekersitestable.h
	gui/widgets/wadseekerwadstable.h
	ini/ini.h
	ini/inisection.h
	ini/inivariable.h
	ini/settingsprovider.h
	ini/settingsproviderqt.h
	ip2c/entities/ip2ccountryinfo.h
	ip2c/ip2c.h
	ip2c/ip2cloader.h
	ip2c/ip2cparser.h
	ip2c/ip2cupdater.h
	irc/chatlogarchive.h
	irc/chatlogrotate.h
	irc/chatlogs.h
	irc/chatnetworknamer.h
	irc/configuration/chatlogscfg.h
	irc/configuration/chatnetworkscfg.h
	irc/configuration/ircconfig.h
	irc/constants/ircresponsetype.h
	irc/entities/ircnetworkentity.h
	irc/entities/ircresponseparseresult.h
	irc/entities/ircuserprefix.h
	irc/ircadapterbase.h
	irc/ircchanneladapter.h
	irc/ircchatadapter.h
	irc/ircclient.h
	irc/ircctcpparser.h
	irc/ircglobal.h
	irc/ircisupportparser.h
	irc/ircmessageclass.h
	irc/ircnetworkadapter.h
	irc/ircnetworkconnectioninfo.h
	irc/ircnicknamecompleter.h
	irc/ircprivadapter.h
	irc/ircrequestparser.h
	irc/ircresponseparser.h
	irc/ircuserinfo.h
	irc/ircuserlist.h
	irc/ops/ircdelayedoperation.h
	irc/ops/ircdelayedoperationban.h
	irc/ops/ircdelayedoperationignore.h
	joincommandlinebuilder.h
	json.h
	localization.h
	localizationinfo.h
	log.h
	lookuphost.h
	main.h
	pathfinder/basefileseeker.h
	pathfinder/caseinsensitivefsfileseeker.h
	pathfinder/casesensitivefsfileseeker.h
	pathfinder/filealias.h
	pathfinder/filesearchpath.h
	pathfinder/pathfind.h
	pathfinder/pathfinder.h
	pathfinder/wadpathfinder.h
	patternlist.h
	plugins/enginedefaults.h
	plugins/engineplugin.h
	plugins/pluginloader.h
	qtmetapointer.h
	random.h
	refresher/canrefreshserver.h
	refresher/hostport.h
	refresher/refresher.h
	refresher/udpsocketpool.h
	scanner.h
	serverapi/broadcast.h
	serverapi/broadcastmanager.h
	serverapi/buddyinfo.h
	serverapi/createserverdialogapi.h
	serverapi/exefile.h
	serverapi/gameclientrunner.h
	serverapi/gamecreateparams.h
	serverapi/gameexefactory.h
	serverapi/gameexeretriever.h
	serverapi/gamefile.h
	serverapi/gamehost.h
	serverapi/joinerror.h
	serverapi/masterclient.h
	serverapi/mastermanager.h
	serverapi/message.h
	serverapi/player.h
	serverapi/playerslist.h
	serverapi/polymorphism.h
	serverapi/rconprotocol.h
	serverapi/server.h
	serverapi/serverlistcounttracker.h
	serverapi/serverptr.h
	serverapi/serverstructs.h
	serverapi/serversummary.h
	serverapi/tooltips/dmflagshtmlgenerator.h
	serverapi/tooltips/gameinfotip.h
	serverapi/tooltips/generalinfotip.h
	serverapi/tooltips/playertable.h
	serverapi/tooltips/servertooltip.h
	serverapi/tooltips/tooltipgenerator.h
	socketsignalsadapter.h
	speedcalculator.h
	strings.hpp
	svnrevision.h
	tests/testbase.h
	tests/testcore.h
	tests/testdatapaths.h
	tests/testini.h
	tests/testircisupportparser.h
	tests/testruns.h
	tests/teststrings.h
	tests/testutf8splitter.h
	updater/autoupdater.h
	updater/updatechannel.h
	updater/updateinstaller.h
	updater/updatepackage.h
	updater/updatepackagefilter.h
	updater/updaterinfoparser.h
	updater/updaterscriptparser.h
	urlopener.h
	utf8splitter.h
	version.h
	versiondump.h
)
set(SOURCE_FILES
	application.cpp
	apprunner.cpp
	clipboard.cpp
	cmdargshelp.cpp
	commandline.cpp
	commandlinetokenizer.cpp
	configuration/doomseekerconfig.cpp
	configuration/passwordscfg.cpp
	configuration/queryspeed.cpp
	configuration/serverpassword.cpp
	configuration/serverpasswordsummary.cpp
	connectionhandler.cpp
	customservers.cpp
	datapaths.cpp
	datastreamoperatorwrapper.cpp
	doomseekerfilepaths.cpp
	filefilter.cpp
	fileutils.cpp
	gamedemo.cpp
	gui/aboutdialog.cpp
	gui/commongui.cpp
	gui/configuration/cfgappearance.cpp
	gui/configuration/cfgautoupdates.cpp
	gui/configuration/cfgcustomservers.cpp
	gui/configuration/cfgfilepaths.cpp
	gui/configuration/cfgquery.cpp
	gui/configuration/cfgserverpasswords.cpp
	gui/configuration/cfgwadalias.cpp
	gui/configuration/cfgwadseekerappearance.cpp
	gui/configuration/cfgwadseekergeneral.cpp
	gui/configuration/cfgwadseekeridgames.cpp
	gui/configuration/cfgwadseekersites.cpp
	gui/configuration/configpage.cpp
	gui/configuration/configurationdialog.cpp
	gui/configuration/doomseekerconfigurationdialog.cpp
	gui/configuration/engineconfigpage.cpp
	gui/configuration/irc/cfgchatlogspage.cpp
	gui/configuration/irc/cfgircappearance.cpp
	gui/configuration/irc/cfgircdefinenetworkdialog.cpp
	gui/configuration/irc/cfgircnetworks.cpp
	gui/configuration/irc/cfgircsounds.cpp
	gui/configuration/irc/ircconfigurationdialog.cpp
	gui/copytextdlg.cpp
	gui/createserver/customparamspanel.cpp
	gui/createserver/dmflagspanel.cpp
	gui/createserver/gameexecutablepicker.cpp
	gui/createserver/gamerulespanel.cpp
	gui/createserver/generalgamesetuppanel.cpp
	gui/createserver/iwadandwadspickerdialog.cpp
	gui/createserver/iwadpicker.cpp
	gui/createserver/maplistpanel.cpp
	gui/createserver/miscserversetuppanel.cpp
	gui/createserver/wadspicker.cpp
	gui/createserverdialog.cpp
	gui/demomanager.cpp
	gui/dockBuddiesList.cpp
	gui/entity/serverlistfilterinfo.cpp
	gui/entity/showmode.cpp
	gui/freedoomdialog.cpp
	gui/helpers/comboboxex.cpp
	gui/helpers/datetablewidgetitem.cpp
	gui/helpers/playersdiagram.cpp
	gui/helpers/taskbarbutton.cpp
	gui/helpers/taskbarprogress.cpp
	gui/ip2cupdatebox.cpp
	gui/irc/ircdock.cpp
	gui/irc/ircdocktabcontents.cpp
	gui/irc/ircignoresmanager.cpp
	gui/irc/ircnetworkselectionbox.cpp
	gui/irc/ircsounds.cpp
	gui/logdock.cpp
	gui/mainwindow.cpp
	gui/missingwadsdialog.cpp
	gui/models/serverlistcolumn.cpp
	gui/models/serverlistmodel.cpp
	gui/models/serverlistproxymodel.cpp
	gui/models/serverlistrowhandler.cpp
	gui/passworddlg.cpp
	gui/programargshelpdialog.cpp
	gui/rconpassworddialog.cpp
	gui/remoteconsole.cpp
	gui/serverdetailsdock.cpp
	gui/serverfilterdock.cpp
	gui/serverlist.cpp
	gui/standardserverconsole.cpp
	gui/wadseekerinterface.cpp
	gui/wadseekershow.cpp
	gui/widgets/colorbutton.cpp
	gui/widgets/createserverdialogpage.cpp
	gui/widgets/dndfilesystemlistview.cpp
	gui/widgets/engineplugincombobox.cpp
	gui/widgets/filepickwidget.cpp
	gui/widgets/fontbutton.cpp
	gui/widgets/iconlabel.cpp
	gui/widgets/importantmessageswidget.cpp
	gui/widgets/memorylineedit.cpp
	gui/widgets/multicombobox.cpp
	gui/widgets/serverconsole.cpp
	gui/widgets/serverfilterbuildermenu.cpp
	gui/widgets/serverlistcontextmenu.cpp
	gui/widgets/serverlistview.cpp
	gui/widgets/serversstatuswidget.cpp
	gui/widgets/tablewidgetmouseaware.cpp
	gui/widgets/wadseekersitestable.cpp
	gui/widgets/wadseekerwadstable.cpp
	ini/ini.cpp
	ini/inisection.cpp
	ini/inivariable.cpp
	ini/settingsprovider.cpp
	ini/settingsproviderqt.cpp
	ip2c/entities/ip2ccountryinfo.cpp
	ip2c/ip2c.cpp
	ip2c/ip2cloader.cpp
	ip2c/ip2cparser.cpp
	ip2c/ip2cupdater.cpp
	irc/chatlogarchive.cpp
	irc/chatlogrotate.cpp
	irc/chatlogs.cpp
	irc/chatnetworknamer.cpp
	irc/configuration/chatlogscfg.cpp
	irc/configuration/chatnetworkscfg.cpp
	irc/configuration/ircconfig.cpp
	irc/constants/ircresponsetype.cpp
	irc/entities/ircnetworkentity.cpp
	irc/entities/ircresponseparseresult.cpp
	irc/entities/ircuserprefix.cpp
	irc/ircadapterbase.cpp
	irc/ircchanneladapter.cpp
	irc/ircchatadapter.cpp
	irc/ircclient.cpp
	irc/ircctcpparser.cpp
	irc/ircglobal.cpp
	irc/ircisupportparser.cpp
	irc/ircmessageclass.cpp
	irc/ircnetworkadapter.cpp
	irc/ircnicknamecompleter.cpp
	irc/ircprivadapter.cpp
	irc/ircrequestparser.cpp
	irc/ircresponseparser.cpp
	irc/ircuserinfo.cpp
	irc/ircuserlist.cpp
	irc/ops/ircdelayedoperation.cpp
	irc/ops/ircdelayedoperationban.cpp
	irc/ops/ircdelayedoperationignore.cpp
	joincommandlinebuilder.cpp
	json.cpp
	localization.cpp
	localizationinfo.cpp
	log.cpp
	lookuphost.cpp
	main.cpp
	pathfinder/basefileseeker.cpp
	pathfinder/caseinsensitivefsfileseeker.cpp
	pathfinder/casesensitivefsfileseeker.cpp
	pathfinder/filealias.cpp
	pathfinder/filesearchpath.cpp
	pathfinder/pathfind.cpp
	pathfinder/pathfinder.cpp
	pathfinder/wadpathfinder.cpp
	patternlist.cpp
	plugins/engineplugin.cpp
	plugins/pluginloader.cpp
	random.cpp
	refresher/canrefreshserver.cpp
	refresher/hostport.cpp
	refresher/refresher.cpp
	refresher/udpsocketpool.cpp
	scanner.cpp
	serverapi/broadcast.cpp
	serverapi/broadcastmanager.cpp
	serverapi/buddyinfo.cpp
	serverapi/createserverdialogapi.cpp
	serverapi/exefile.cpp
	serverapi/gameclientrunner.cpp
	serverapi/gamecreateparams.cpp
	serverapi/gameexefactory.cpp
	serverapi/gameexeretriever.cpp
	serverapi/gamefile.cpp
	serverapi/gamehost.cpp
	serverapi/joinerror.cpp
	serverapi/masterclient.cpp
	serverapi/mastermanager.cpp
	serverapi/message.cpp
	serverapi/player.cpp
	serverapi/playerslist.cpp
	serverapi/rconprotocol.cpp
	serverapi/server.cpp
	serverapi/serverlistcounttracker.cpp
	serverapi/serverstructs.cpp
	serverapi/serversummary.cpp
	serverapi/tooltips/dmflagshtmlgenerator.cpp
	serverapi/tooltips/gameinfotip.cpp
	serverapi/tooltips/generalinfotip.cpp
	serverapi/tooltips/playertable.cpp
	serverapi/tooltips/servertooltip.cpp
	serverapi/tooltips/tooltipgenerator.cpp
	speedcalculator.cpp
	strings.cpp
	tests/testbase.cpp
	tests/testcore.cpp
	tests/testdatapaths.cpp
	tests/testini.cpp
	tests/testircisupportparser.cpp
	tests/testruns.cpp
	tests/teststrings.cpp
	tests/testutf8splitter.cpp
	updater/autoupdater.cpp
	updater/updatechannel.cpp
	updater/updateinstaller.cpp
	updater/updatepackagefilter.cpp
	updater/updaterinfoparser.cpp
	updater/updaterscriptparser.cpp
	urlopener.cpp
	utf8splitter.cpp
	version.cpp
	versiondump.cpp
)
