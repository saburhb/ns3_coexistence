#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "ns3/applications-module.h"

using namespace ns3;

#define const char* getfield(char* line, int num);

class MyApp : public Application
{
public:

  MyApp (FILE *fpt);
  virtual ~MyApp();

  void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);
  void ChangeRate(DataRate newrate);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void ScheduleTx (double);
  void SendPacket (void);

  Ptr<Socket>     m_socket;
  Address         m_peer;
  uint32_t        m_packetSize;
  uint32_t        m_nPackets;
  DataRate        m_dataRate;
  EventId         m_sendEvent;
  bool            m_running;
  uint32_t        m_packetsSent;
  FILE *          m_fp;
  double          m_prev;
};


class MyVidApp : public Application
{
public:

  MyVidApp (FILE *fpt);
  virtual ~MyVidApp();

  void SetupVid (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate, uint32_t appId);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void ScheduleTx (double);
  void SendPacket (void);

  Ptr<Socket>     m_socket;
  Address         m_peer;
  uint32_t        m_packetSize;
  uint32_t        m_nPackets;
  DataRate        m_dataRate;
  EventId         m_sendEventVid;
  bool            m_running;
  uint32_t        m_numpacketsSent;
  FILE *          m_fp;
  double          m_prev;
  uint32_t	  m_appId;
};



class MyFtpApp : public Application
{
public:

  MyFtpApp (FILE *fpt);
  virtual ~MyFtpApp();

  void SetupFtp (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate, uint32_t m_appId);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void ScheduleTx (double);
  void SendPacket (void);

  Ptr<Socket>     m_socket;
  Address         m_peer;
  uint32_t        m_packetSize;
  uint32_t        m_nPackets;
  DataRate        m_dataRate;
  EventId         m_sendEventFtp;
  bool            m_running;
  uint32_t        m_packetsSent;
  FILE *          m_fp;
  double          m_prev;
  uint32_t        m_appId;
};

