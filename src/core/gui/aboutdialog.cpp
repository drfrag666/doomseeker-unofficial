//------------------------------------------------------------------------------
// aboutdialog.cpp
//------------------------------------------------------------------------------
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301  USA
//
//------------------------------------------------------------------------------
// Copyright (C) 2009 Braden "Blzut3" Obrzut <admin@maniacsvault.net>
//------------------------------------------------------------------------------
#include "aboutdialog.h"

#include "gui/copytextdlg.h"
#include "plugins/engineplugin.h"
#include "plugins/pluginloader.h"
#include "ui_aboutdialog.h"
#include "wadseeker/wadseekerversioninfo.h"
#include "version.h"
#include <QPixmap>
#include <QResource>
#include <QString>

DClass<AboutDialog> : public Ui::AboutDialog
{
};

DPointered(AboutDialog)

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
	d->setupUi(this);

	connect(d->buttonBox, SIGNAL( clicked(QAbstractButton *) ), SLOT( close() ));

	// Doomseeker
	d->versionChangeset->setText(Version::changeset());
	d->versionNumber->setText(Version::versionRevision());
	d->lblRevision->setText(QString::number(Version::revisionNumber()));
	d->logo->setPixmap(QPixmap(":/logo.png"));

	// Wadseeker
	d->wadseekerAuthor->setText(WadseekerVersionInfo::author());
	d->wadseekerDescription->setText(WadseekerVersionInfo::description());
	d->wadseekerVersion->setText(WadseekerVersionInfo::version());
	d->wadseekerYearSpan->setText(WadseekerVersionInfo::yearSpan());

	// Populate plugins dialog
	for(unsigned i = 0; i < gPlugins->numPlugins(); ++i)
	{
		d->pluginBox->addItem( gPlugins->plugin(i)->info()->data()->name);
	}
	connect(d->pluginBox, SIGNAL( currentIndexChanged(int) ), SLOT( changePlugin(int) ));
	changePlugin(0);

	d->jsonLayout->setAlignment(d->btnJsonLicense, Qt::AlignTop);
	adjustSize();
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::changePlugin(int pluginIndex)
{
	if(static_cast<unsigned> (pluginIndex) >= gPlugins->numPlugins())
		return; // Invalid plugin.

	const EnginePlugin* plug = gPlugins->plugin(pluginIndex)->info();

	d->pluginAuthor->setText(plug->data()->author);
	d->pluginVersion->setText(QString("Version: %1.%2").arg(plug->data()->abiVersion).arg(plug->data()->version));
}

void AboutDialog::showJsonLicense()
{
	QResource license = QResource("LICENSE.json");
	QString licenseText = QString::fromUtf8(
		reinterpret_cast<const char*>(license.data()),
		license.size());
	CopyTextDlg dialog = CopyTextDlg(licenseText, tr("JSON library license"), this);
	dialog.resize(550, dialog.height());
	dialog.exec();
}
