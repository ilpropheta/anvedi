#include "RTSocketPlayer.h"
#include "SignalData.h"
#include <QMessageBox>
#include <sstream>

using namespace std;

RTSocketPlayer::RTSocketPlayer(SignalData& data)
	: m_data(data), sender(m_data)
{
	if (!server.listen("\\\\.\\pipe\\anvedi"))
	{
		QMessageBox::critical(nullptr, tr("Anvedi Server"),
			tr("Unable to start the server: %1.")
			.arg(server.errorString()));
	}
	else
	{
		QObject::connect(&server, SIGNAL(newConnection()), this, SLOT(Start()));
	}

	sender.setPacketCount(100);
}

void RTSocketPlayer::Start()
{
	clientConnection = server.nextPendingConnection();

	// blocking - temporary
	if (clientConnection->waitForReadyRead())
	{
		auto block = clientConnection->readAll();
		auto str = block.toStdString();
		stringstream ss(str, ios::binary | ios::in);
		
		string domainName;
		DataMap data;
		names.clear();

		// number of signals
		int numberOfSignals;
		ss.read((char*) &numberOfSignals, sizeof(int));
		
		// names
		int currSignNameSize;
		for (auto i = 0; i < numberOfSignals; ++i)
		{
			ss.read((char*) &currSignNameSize, sizeof(int));
			vector<char> currName(currSignNameSize);
			ss.read((char*) currName.data(), currName.size());
			if (i == 0)
				domainName.assign(currName.begin(), currName.end());
			QString currNameAsQString(string(currName.begin(), currName.end()).c_str());
			data.emplace(currNameAsQString, Signal{currNameAsQString});
			names.push_back(currNameAsQString);
		}
		for (auto& signal : data)
		{
			m_data.addIfNotExists(std::move(signal.second));
		}
		m_data.setAsDomain(domainName.c_str());
	}
	else
	{
		QMessageBox::critical(nullptr, tr("Anvedi Server"),
			tr("Unable to receive data from client")
		);
		clientConnection->disconnectFromServer();
		return;
	}


	connect(clientConnection, &QLocalSocket::readyRead, [this]{
		auto block = clientConnection->readAll();
		auto str = block.toStdString();
		stringstream ss(str, ios::binary | ios::in);
		unsigned int samplesCount;
		ss.read((char*) &samplesCount, sizeof(unsigned int));
		
		vector<pair<QString, QVector<qreal>>> slice;

		for (auto i = 0; i < names.size(); ++i)
		{
			QVector<qreal> samples(samplesCount);
			for (auto j = 0; j < samplesCount; ++j)
			{
				ss.read((char*) &samples[j], sizeof(qreal));
			}

			slice.emplace_back(names[i], std::move(samples));
		}
		
		sender.SetDataToSend(std::move(slice));
		sender.SendData();
	});
}

void RTSocketPlayer::Stop()
{
	clientConnection->disconnectFromServer();
}
