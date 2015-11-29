#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "ns3/applications-module.h"
#include "myApps.h"

using namespace ns3;


const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

/********************* TCP Application **********************/

MyApp::MyApp (FILE *fpt)
  : m_socket (0),
    m_peer (),
    m_packetSize (0),
    m_nPackets (0),
    m_dataRate (0),
    m_sendEvent (),
    m_running (false),
    m_packetsSent (0),
    m_fp(fpt),
    m_prev (0)
{
}

MyApp::~MyApp()
{
  m_socket = 0;
}


void
MyApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate)
{
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_nPackets = nPackets;
  m_dataRate = dataRate;
}

void
MyApp::StartApplication (void)
{
  m_running = true;
  m_packetsSent = 0;
  m_socket->Bind ();
  m_socket->Connect (m_peer);
  SendPacket ();
}

void
MyApp::StopApplication (void)
{
  m_running = false;

  if (m_sendEvent.IsRunning ())
    {
      Simulator::Cancel (m_sendEvent);
    }

  if (m_socket)
    {
      m_socket->Close ();
    }
}

void
MyApp::SendPacket (void)
{
	double curr=0, inter_arrival=0.5;
  	int length=0;
  	char line[1024];
        char *sTime=(char *)malloc(sizeof(char) *30);
	char *sLength=(char *)malloc(sizeof(char) *10);

	fgets(line, 1024, m_fp);
	char *tmp1 = strdup(line);
    	char *tmp2 = strdup(line);

	sTime=getfield(tmp1, 1);
	sLength=getfield(tmp2, 2);
    	curr = atof(sTime);
        length=atoi(sLength);

	if(m_prev==0)
        	inter_arrival=0;
	else{
		inter_arrival = curr-m_prev;
	}
		m_prev=curr;

	SeqTsHeader seqTs;
        seqTs.SetSeq (m_packetsSent);

	Ptr<Packet> packet = Create<Packet> (length);
	packet->AddHeader (seqTs);
  	m_socket->Send (packet);

	std::cout << "SEND:" << Simulator::Now ().GetSeconds () << "\t" << "Youtube: " << m_packetsSent << "\t" << length << "\t" << "to :" << InetSocketAddress::ConvertFrom(m_peer).GetIpv4 () <<"\n";

  	if (++m_packetsSent < m_nPackets)
    	{
      		ScheduleTx (inter_arrival);
    	}


}

void
MyApp::ScheduleTx (double interval)
{
  if (m_running)
    {
      Time tNext (Seconds (interval));
      m_sendEvent = Simulator::Schedule (tNext, &MyApp::SendPacket, this);

    }
}

void
MyApp::ChangeRate(DataRate newrate)
{
   m_dataRate = newrate;
   return;
}


/*********************** END TCP Application *************************/

/********************** Create UDP Application ********************/

MyVidApp::MyVidApp (FILE *fpt)
  : m_socket (0),
    m_peer (),
    m_packetSize (0),
    m_nPackets (0),
    m_dataRate (0),
    m_sendEventVid (),
    m_running (false),
    m_numpacketsSent (0),
    m_fp(fpt),
    m_prev(0),
    m_appId(0)
{
}
 
MyVidApp::~MyVidApp()
{
  m_socket = 0;
}

void
MyVidApp::SetupVid (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate, uint32_t appId)
{
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_nPackets = nPackets;
  m_dataRate = dataRate;
  m_appId = appId;
}

void
MyVidApp::StartApplication (void)
{
  m_running = true;
  m_numpacketsSent = 0;
  m_socket->Bind ();
  m_socket->Connect (m_peer);
  SendPacket ();
}

void
MyVidApp::StopApplication (void)
{
  m_running = false;

  if (m_sendEventVid.IsRunning ())
    {
      Simulator::Cancel (m_sendEventVid);
    }

  if (m_socket)
    {
      m_socket->Close ();
    }
}

void
MyVidApp::SendPacket (void)
{
        double curr=0, inter_arrival=0.5;
        int length=0;
        char line[1024];

	
  	if(m_appId == 0){
		length=1500;
		inter_arrival=0.001;
	}
	else{
        	char *sTime=(char *)malloc(sizeof(char) *30);
        	char *sLength=(char *)malloc(sizeof(char) *10);

        	fgets(line, 1024, m_fp);
        	char *tmp1 = strdup(line);
        	char *tmp2 = strdup(line);

        	sTime=getfield(tmp1, 1);
        	sLength=getfield(tmp2, 2);
        	curr = atof(sTime);
        	length=atoi(sLength);

        	if(m_prev==0)
                	inter_arrival=0;
        	else{
                	inter_arrival = curr-m_prev;
        	}
                m_prev=curr;
	}

	SeqTsHeader seqTs;
        seqTs.SetSeq (m_numpacketsSent);

        Ptr<Packet> packet = Create<Packet> (length);
	packet->AddHeader (seqTs);
        m_socket->Send (packet);

	char vidApp[20]="";
	if(m_appId == 1){
		strcpy(vidApp, "Skype: ");
	}
	else if (m_appId == 2){
                strcpy(vidApp, "Vid: ");
        }
	else if (m_appId == 0){
                strcpy(vidApp, "Flood: ");
        }

	std::cout << "SEND:" << Simulator::Now ().GetSeconds () << "\t" << vidApp << m_numpacketsSent << "\t" << length << "\t" << "to :" << InetSocketAddress::ConvertFrom(m_peer).GetIpv4 () <<"\n";
        if (++m_numpacketsSent < m_nPackets)
        {
                ScheduleTx (inter_arrival);
        }
}

void
MyVidApp::ScheduleTx (double interval)
{
  if (m_running)
    {
      //Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ())));
      Time tNext (Seconds (interval));
      m_sendEventVid = Simulator::Schedule (tNext, &MyVidApp::SendPacket, this);
    }
}


/*********************** END UDP Application **********************/

/********************** Create TCP FTP Application ********************/

MyFtpApp::MyFtpApp (FILE *fpt)
  : m_socket (0),
    m_peer (),
    m_packetSize (0),
    m_nPackets (0),
    m_dataRate (0),
    m_sendEventFtp (),
    m_running (false),
    m_packetsSent (0),
    m_fp(fpt),
    m_prev(0),
    m_appId(0)
{
}

MyFtpApp::~MyFtpApp()
{
  m_socket = 0;
}

void
MyFtpApp::SetupFtp (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate, uint32_t appId)
{
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_nPackets = nPackets;
  m_dataRate = dataRate;
  m_appId = appId;
}

void
MyFtpApp::StartApplication (void)
{
  m_running = true;
  m_packetsSent = 0;
  m_socket->Bind ();
  m_socket->Connect (m_peer);
  SendPacket ();
}

void
MyFtpApp::StopApplication (void)
{
  m_running = false;

  if (m_sendEventFtp.IsRunning ())
    {
      Simulator::Cancel (m_sendEventFtp);
    }

  if (m_socket)
    {
      m_socket->Close ();
    }
}

void
MyFtpApp::SendPacket (void)
{
        double curr=0, inter_arrival=0.5;
        int length=0;
        char line[1024];
        char *sTime=(char *)malloc(sizeof(char) *30);
        char *sLength=(char *)malloc(sizeof(char) *10);

        fgets(line, 1024, m_fp);
        char *tmp1 = strdup(line);
        char *tmp2 = strdup(line);

        sTime=getfield(tmp1, 1);
        sLength=getfield(tmp2, 2);
        curr = atof(sTime);
        length=atoi(sLength);

        if(m_prev==0)
                inter_arrival=0;
        else{
                inter_arrival = curr-m_prev;
        }
                m_prev=curr;

	SeqTsHeader seqTs;
        seqTs.SetSeq (m_packetsSent);

        Ptr<Packet> packet = Create<Packet> (length);
        packet->AddHeader (seqTs);
        m_socket->Send (packet);

	char ftpApp[20]="";
	sprintf(ftpApp, "FTP%d: ", m_appId);

	std::cout << "SEND:" << Simulator::Now ().GetSeconds () << "\t" << ftpApp << m_packetsSent << "\t" << length << "\t" << "to :" << InetSocketAddress::ConvertFrom(m_peer).GetIpv4 () << "\n";
        if (++m_packetsSent < m_nPackets)
        {
                ScheduleTx (inter_arrival);
        }
}

void
MyFtpApp::ScheduleTx (double interval)
{
  if (m_running)
    {
      Time tNext (Seconds (interval));
      m_sendEventFtp = Simulator::Schedule (tNext, &MyFtpApp::SendPacket, this);
    }
}

/*********************** END TCP FTP Application **********************/

