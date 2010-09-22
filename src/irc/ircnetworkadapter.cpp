//------------------------------------------------------------------------------
// ircnetworkadapter.cpp
//
// Copyright (C) 2010 "Zalewa" <zalewapl@gmail.com>
//------------------------------------------------------------------------------
#include "ircnetworkadapter.h"
#include "irc/ircchanneladapter.h"
#include "irc/ircglobal.h"
#include "irc/ircprivadapter.h"
#include "irc/ircrequestparser.h"
#include "irc/ircuserinfo.h"
#include "log.h"

IRCNetworkAdapter::IRCNetworkAdapter()
{
	pIrcSocketSignalsAdapter = new IRCSocketSignalsAdapter(this);
	ircClient.connectSocketSignals(pIrcSocketSignalsAdapter);
	
	QObject::connect(&ircClient, SIGNAL( ircServerResponse(const QString&) ), 
		this, SLOT( ircServerResponse(const QString&) ) );


	// Response parser begins here.
	QObject::connect(&ircResponseParser, SIGNAL( kick(const QString&, const QString&, const QString&, const QString&) ), 
		this, SLOT( kick(const QString&, const QString&, const QString&, const QString&) ) );
		
	QObject::connect(&ircResponseParser, SIGNAL( kill(const QString&, const QString&) ), 
		this, SLOT( kill(const QString&, const QString&) ) );

	QObject::connect(&ircResponseParser, SIGNAL( modeInfo(const QString&, const QString&, const QString&) ), 
		this, SLOT( modeInfo(const QString&, const QString&, const QString&) ) );

	QObject::connect(&ircResponseParser, SIGNAL( namesListReceived(const QString&, const QStringList&) ), 
		this, SLOT( namesListReceived(const QString&, const QStringList&) ) );

	QObject::connect(&ircResponseParser, SIGNAL( namesListEndReceived(const QString&) ), 
		this, SLOT( namesListEndReceived(const QString&) ) );

	QObject::connect(&ircResponseParser, SIGNAL ( parseError(const QString&) ), 
		this, SLOT( parseError(const QString&) ) );

	QObject::connect(&ircResponseParser, SIGNAL ( privMsgReceived(const QString&, const QString&, const QString&) ), 
		this, SLOT( privMsgReceived(const QString&, const QString&, const QString&) ) );

	QObject::connect(&ircResponseParser, SIGNAL ( sendPongMessage(const QString&) ), 
		this, SLOT( sendPong(const QString&) ) );

	QObject::connect(&ircResponseParser, SIGNAL ( userChangesNickname(const QString&, const QString&) ), 
		this, SLOT( userChangesNickname(const QString&, const QString&) ) );

	QObject::connect(&ircResponseParser, SIGNAL ( userJoinsChannel(const QString&, const QString&, const QString&) ), 
		this, SLOT( userJoinsChannel(const QString&, const QString&, const QString&) ) );
		
	QObject::connect(&ircResponseParser, SIGNAL ( userModeChanged(const QString&, const QString&, unsigned, unsigned) ), 
		this, SLOT( userModeChanged(const QString&, const QString&, unsigned, unsigned) ) );		

	QObject::connect(&ircResponseParser, SIGNAL ( userPartsChannel(const QString&, const QString&, const QString&) ), 
		this, SLOT( userPartsChannel(const QString&, const QString&, const QString&) ) );

	QObject::connect(&ircResponseParser, SIGNAL ( userQuitsNetwork(const QString&, const QString&) ), 
		this, SLOT( userQuitsNetwork(const QString&, const QString&) ) );
}

IRCNetworkAdapter::~IRCNetworkAdapter()
{
	disconnect();

	killAllChatWindows();
	delete pIrcSocketSignalsAdapter;
}

void IRCNetworkAdapter::connect(const IRCNetworkConnectionInfo& connectionInfo)
{
	this->connectionInfo = connectionInfo;
	emit titleChange();
	ircClient.connect(connectionInfo.serverAddress, connectionInfo.serverPort);
}

void IRCNetworkAdapter::detachChatWindow(const IRCChatAdapter* pAdapter)
{
	chatWindows.remove(pAdapter->recipient());
}

void IRCNetworkAdapter::disconnect(const QString& farewellMessage)
{
	sendMessage("/quit " + farewellMessage);
	ircClient.disconnect();	
}

void IRCNetworkAdapter::doSendMessage(const QString& message, IRCAdapterBase* pOrigin)
{
	if (pOrigin == NULL)
	{
		pOrigin = this;
	}
	
	if (!ircClient.isConnected())
	{
		pOrigin->emitError(tr("You are not connected to the network."));
		return;
	}

	QString parsedMessage;
	IRCRequestParser::IRCRequestParseResult result = IRCRequestParser::parse(message, parsedMessage);
	
	switch (result)
	{
		case IRCRequestParser::ErrorInputInsufficientParameters:
			pOrigin->emitError(tr("Insufficient parameters."));
			break;
		
		case IRCRequestParser::ErrorInputNotPrependedWithSlash:
			emit error(tr("This is a server window. All commands must be prepended with a '/' character."));
			break;
			
		case IRCRequestParser::ErrorMessageEmpty:
			pOrigin->emitError(tr("Attempted to send empty message"));
			break;
		
		case IRCRequestParser::ErrorMessageTooLong:
			pOrigin->emitError(tr("Command is too long"));
			break;
			
		case IRCRequestParser::Ok:
			ircClient.sendMessage(parsedMessage);
			break;
			
		case IRCRequestParser::QuitCommand:
			ircClient.sendMessage(parsedMessage);
			emit messageColored(tr("Quit"), IRCGlobal::COLOR_NETWORK_ACTION);
			break;
	}
}

IRCChatAdapter* IRCNetworkAdapter::getOrCreateNewChatAdapter(const QString& recipient)
{
	IRCChatAdapter* pAdapter = NULL;

	if (recipient.isEmpty())
	{
		emit error("Doomseeker error: getOrCreateNewChatAdapter() received empty recipient");
		return NULL;
	}

	QString recipientLowercase = recipient.toLower();
	
	if (hasRecipient(recipientLowercase))
	{
		return chatWindows[recipient];
	}

#ifdef _DEBUG
	Log::instance << QString("IRCNetworkAdapter::getOrCreateNewChatAdapter() Creating new adapter for recipient: %1").arg(recipientLowercase);
#endif
	
	if (IRCGlobal::isChannelName(recipient))
	{
		pAdapter = new IRCChannelAdapter(this, recipient);
	}
	else
	{
		pAdapter = new IRCPrivAdapter(this, recipient);
	}

	chatWindows.insert(recipientLowercase, pAdapter);
	emit newChatWindowIsOpened(pAdapter);

	return pAdapter;
}

bool IRCNetworkAdapter::hasRecipient(const QString& recipient) const
{
	QString recipientLowercase = recipient.toLower();

	return (chatWindows.find(recipient) != chatWindows.end());
}

void IRCNetworkAdapter::ircServerResponse(const QString& message)
{
	ircResponseParser.parse(message);

	emit this->message(message.trimmed());
}

bool IRCNetworkAdapter::isMyNickname(const QString& nickname) const
{
	IRCUserInfo myUserInfo(this->connectionInfo.nick);

	return (myUserInfo == nickname);
}

void IRCNetworkAdapter::kick(const QString& channel, const QString& byWhom, const QString& whoIsKicked, const QString& reason)
{
	IRCChannelAdapter* pAdapter = (IRCChannelAdapter*) this->getOrCreateNewChatAdapter(channel);
	pAdapter->emitMessageColored(tr("%1 was kicked by %2 (%3)").arg(whoIsKicked, byWhom, reason), IRCGlobal::COLOR_CHANNEL_ACTION);
	pAdapter->removeNameFromCachedList(whoIsKicked);
}

void IRCNetworkAdapter::kill(const QString& victim, const QString& comment)
{
	emit message(QString("Connection for user %1 was killed. (%2)").arg(victim, comment));

	// We need to iterate through EVERY adapter and notify them
	// about this quit.
	// Implementation of each adapter should recognize if this quit actually
	// has anything to do with that adapter.
	QList<IRCChatAdapter*> adaptersList = chatWindows.values();
	foreach (IRCChatAdapter* pAdapter, adaptersList)
	{
		pAdapter->userLeaves(victim, comment, IRCChatAdapter::NetworkKill);
	}
}

void IRCNetworkAdapter::killAllChatWindows()
{	
	QList<IRCChatAdapter*> pWindows = chatWindows.values();
	foreach (IRCChatAdapter* pAdapter, pWindows)
	{
		// Make sure that the adapter destructor won't call the
		// detachChatWindow() method or the program will be shot to oblivion.
		pAdapter->setNetwork(NULL);
		delete pAdapter;
	}

	chatWindows.clear();
}

void IRCNetworkAdapter::killChatWindow(const QString& recipient)
{
	if (hasRecipient(recipient))
	{
		IRCChatAdapter* pAdapter = chatWindows[recipient];
		chatWindows.remove(recipient);
		
		// Make sure that the adapter destructor won't call the
		// detachChatWindow() method or the program will be shot to oblivion.
		pAdapter->setNetwork(NULL);
		delete pAdapter;
	}
}

void IRCNetworkAdapter::modeInfo(const QString& channel, const QString& whoSetThis, const QString& modeParams)
{
	IRCChannelAdapter* pAdapter = (IRCChannelAdapter*) this->getOrCreateNewChatAdapter(channel);
	pAdapter->emitMessageColored(tr("%1 sets mode: %2").arg(whoSetThis, modeParams), IRCGlobal::COLOR_CHANNEL_ACTION);
}

void IRCNetworkAdapter::namesListReceived(const QString& channel, const QStringList& names)
{
	IRCChannelAdapter* pAdapter = (IRCChannelAdapter*) this->getOrCreateNewChatAdapter(channel);
	pAdapter->appendNamesToCachedList(names);
}

void IRCNetworkAdapter::namesListEndReceived(const QString& channel)
{
	IRCChannelAdapter* pAdapter = (IRCChannelAdapter*) this->getOrCreateNewChatAdapter(channel);
	pAdapter->emitCachedNameListUpdated();
}

void IRCNetworkAdapter::parseError(const QString& error)
{
	emit this->error(tr("IRC Parse error: %1").arg(error));
}

void IRCNetworkAdapter::privMsgReceived(const QString& recipient, const QString& sender, const QString& content)
{
	IRCChatAdapter* pAdapter = this->getOrCreateNewChatAdapter(recipient);
	pAdapter->emitChatMessage(sender, content);
}

void IRCNetworkAdapter::sendPong(const QString& toWhom)
{
	QString message = QString("/PONG %1").arg(toWhom);
	sendMessage(message);
}

QString IRCNetworkAdapter::title() const
{
	return connectionInfo.serverAddress;
}

void IRCNetworkAdapter::userChangesNickname(const QString& oldNickname, const QString& newNickname)
{
	emit message(QString("User changes nickname: %1 to %2").arg(oldNickname, newNickname));

	if (isMyNickname(oldNickname))
	{
		emit message("Updating own nickname");
		connectionInfo.nick = newNickname;
	}
	
	QList<IRCChatAdapter*> adaptersList = chatWindows.values();
	foreach (IRCChatAdapter* pAdapter, adaptersList)
	{
		pAdapter->userChangesNickname(oldNickname, newNickname);
	}
	
	// MAKE SURE TO SEE IF WE HAVE A CHAT WINDOW OPEN WITH THIS 
	// USER AND FIX THE KEY IN THE MAP!!!
	if (hasRecipient(oldNickname))
	{
		QString oldNicknameLowercase = oldNickname.toLower();
		QString newNicknameLowercase = newNickname.toLower();

		IRCChatAdapter* pAdapter = chatWindows[oldNicknameLowercase];
		chatWindows.remove(oldNicknameLowercase);
		chatWindows.insert(newNicknameLowercase, pAdapter);
	}
}

void IRCNetworkAdapter::userJoinsChannel(const QString& channel, const QString& nickname, const QString& fullSignature)
{
	if (hasRecipient(channel))
	{
		emit messageColored(tr("User %1 joins channel %2").arg(nickname, channel), IRCGlobal::COLOR_CHANNEL_ACTION);

		IRCChannelAdapter* pChannel = (IRCChannelAdapter*)this->getOrCreateNewChatAdapter(channel);
		if (!isMyNickname(nickname))
		{
			pChannel->userJoins(nickname, fullSignature);
		}
	}
	else
	{
		emit error(tr("User %1 joins channel %2, but we are not on this channel!").arg(nickname, channel));
	}
}

void IRCNetworkAdapter::userModeChanged(const QString& channel, const QString& nickname, unsigned flagsAdded, unsigned flagsRemoved)
{
	if (hasRecipient(channel))
	{
		IRCChatAdapter* pAdapter = this->getOrCreateNewChatAdapter(channel);
		pAdapter->userModeChanges(nickname, flagsAdded, flagsRemoved);
	}
}

void IRCNetworkAdapter::userPartsChannel(const QString& channel, const QString& nickname, const QString& farewellMessage)
{
	if (hasRecipient(channel))
	{
		IRCChannelAdapter* pChannel = (IRCChannelAdapter*)chatWindows[channel];
			
		if (isMyNickname(nickname))
		{
			emit messageColored(tr("You left channel %1").arg(channel), IRCGlobal::COLOR_CHANNEL_ACTION);
			killChatWindow(channel);
		}
		else
		{
			emit messageColored(tr("User %1 parts channel %2").arg(nickname, channel), IRCGlobal::COLOR_CHANNEL_ACTION);
			pChannel->userLeaves(nickname, farewellMessage, IRCChatAdapter::ChannelPart);
		}
	}
	
	
}

void IRCNetworkAdapter::userQuitsNetwork(const QString& nickname, const QString& farewellMessage)
{
	emit messageColored(QString("User %1 quits network.").arg(nickname), IRCGlobal::COLOR_NETWORK_ACTION);
	
	// We need to iterate through EVERY adapter and notify them
	// about this quit.
	// Implementation of each adapter should recognize if this quit actually
	// has anything to do with that adapter.
	QList<IRCChatAdapter*> adaptersList = chatWindows.values();
	foreach (IRCChatAdapter* pAdapter, adaptersList)
	{
		pAdapter->userLeaves(nickname, farewellMessage, IRCChatAdapter::NetworkQuit);
	}
}

////////////////////////////////////////////////////////////////////////////////

void IRCSocketSignalsAdapter::connected()
{
	emit pParent->message(tr("Connected. Sending registration messages."));
	
	IRCNetworkConnectionInfo& connectionInfo = pParent->connectionInfo;
	
	QString messagePass = "/PASS %1";
	QString messageNick = "/NICK %1 %2";
	QString messageUser = "/USER %1 %2 %3 :%4";
	
	if (!connectionInfo.serverPassword.isEmpty())
	{
		pParent->sendMessage(messagePass.arg(connectionInfo.serverPassword));
	}
	
	pParent->sendMessage(messageNick.arg(connectionInfo.nick).arg(connectionInfo.alternateNick));
	pParent->sendMessage(messageUser.arg(connectionInfo.nick).arg(connectionInfo.nick).arg(connectionInfo.nick).arg(connectionInfo.realName));
	pParent->sendMessage(messageUser.arg(connectionInfo.nick));
}

void IRCSocketSignalsAdapter::disconnected()
{
	pParent->killAllChatWindows();
	gLog << tr("IRC: Disconnected from network %1").arg(pParent->connectionInfo.serverAddress);
	emit pParent->message("Disconnected");
}

void IRCSocketSignalsAdapter::errorReceived(QAbstractSocket::SocketError error)
{
	emit pParent->error(pSocket->errorString());
}

void IRCSocketSignalsAdapter::infoMessage(const QString& message)
{
	gLog << message;
	emit pParent->message(message);
}

void IRCSocketSignalsAdapter::hostLookupError(QHostInfo::HostInfoError errorValue)
{
	switch (errorValue)
	{
		case QHostInfo::HostNotFound:
			emit pParent->error("Host lookup error: host not found.");
			break;
			
		default:
			emit pParent->error("Unknown host lookup error.");
			break;
	}
}
