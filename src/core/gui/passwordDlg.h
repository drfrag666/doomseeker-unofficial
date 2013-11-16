//------------------------------------------------------------------------------
// passwordDlg.h
//------------------------------------------------------------------------------
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//
//------------------------------------------------------------------------------
// Copyright (C) 2009 "Blzut3" <admin@maniacsvault.net>
//------------------------------------------------------------------------------

#ifndef __PASSWORDDIALOG_H__
#define __PASSWORDDIALOG_H__

#include "ui_passwordDlg.h"
#include <QStringList>

class EnginePlugin;

class PasswordDlg : public QDialog, private Ui::passwordDlg
{
	Q_OBJECT

	public:
		PasswordDlg(QWidget *parent=NULL, bool rcon=false, bool connection=false);

		QStringList allConnectPasswords() const;
		QString connectPassword() const;
		const EnginePlugin *selectedEngine() const;
		QString serverAddress() const { return leServerAddress->text(); }
		void setCurrentConnectPassword(const QString& password);
		void setPasswords(const QStringList& passwords);

	public slots:
		void accept();
		void removeCurrentConnectPassword();

	private:
		bool rcon;

		void loadPasswords();
};

#endif /* __PASSWORDDIALOG_H__ */
