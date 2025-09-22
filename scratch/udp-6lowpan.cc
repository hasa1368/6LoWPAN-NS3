
#include "ns3/node-container.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-module.h"
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/energy-source-container.h"
#include "ns3/acoustic-modem-energy-model-helper.h"
#include <iostream>
using namespace ns3;
AnimationInterface * pAnim = 0;
uint32_t resourceId1;
uint32_t resourceId2;
uint32_t resourceId3;
uint32_t resourceId4;
uint32_t nodeCounterIdUint32;
uint32_t nodeCounterIdDouble1;
uint32_t nodeCounterIdDouble2;
/// RGB structure
struct rgb {
  uint8_t r; ///< red
  uint8_t g; ///< green
  uint8_t b; ///< blue
};

struct rgb colors [] = {
                        { 255, 0, 0 }, // Red
                        { 0, 255, 0 }, // Blue
                        { 0, 0, 255 }  // Green
                        };












NS_LOG_COMPONENT_DEFINE ("ExampleSixlowpan");

int main (int argc, char** argv)
{

std::string animFile = "6lowpan/6lowpan.xml" ;  // Name of file for animation output
  bool verbose = false;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "turn on some relevant log components", verbose);
  cmd.Parse (argc, argv);

  if (verbose)
    {
      LogComponentEnable ("SixLowPanNetDevice", LOG_LEVEL_ALL);
      LogComponentEnable ("Ping6Application", LOG_LEVEL_ALL);
    }

  Packet::EnablePrinting ();
  Packet::EnableChecking ();

  NS_LOG_INFO ("Create nodes.");
  //Ptr<Node> n0 = CreateObject<Node> ();
  //Ptr<Node> r = CreateObject<Node> ();
  //Ptr<Node> n1 = CreateObject<Node> ();
  NodeContainer n0;
  n0.Create (1);
   NodeContainer n01;
  n01.Create (1);
   NodeContainer n02;
  n02.Create (1);
  NodeContainer n03;
  n03.Create (1);
 NodeContainer n04;
  n04.Create (1);
 NodeContainer n05;
  n05.Create (1);



  NodeContainer r;
  r.Create (1);
  NodeContainer n1;
  n1.Create (1);
  //MobilityHelper mobilityHelper;
   Ns2MobilityHelper ns2 = Ns2MobilityHelper ("scratch/Scenario.tcl");
   ns2.Install ();



  BasicEnergySourceHelper energySourceHelper;
  energySourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (10));
  energySourceHelper.Install (n0);




  NodeContainer net1 (n0, r);
  NodeContainer net01 (n01, r);
  NodeContainer net02 (n02, r);
  NodeContainer net03 (n03, r);
  NodeContainer net04 (n05, r);
  NodeContainer net2 (r, n1);
  NodeContainer all (n0, r, n1);

  NS_LOG_INFO ("Create IPv6 Internet Stack");
  InternetStackHelper internetv6;
   InternetStackHelper internetv61;
  internetv6.Install (all);
  internetv61.Install (n01);
  internetv61.Install (n02);
  internetv61.Install (n03);
  internetv61.Install (n05);

  LrWpanHelper lrWpanHelper;     
  NetDeviceContainer lrwpanDevices = lrWpanHelper.Install(all); 
    
  NS_LOG_INFO ("Create channels.");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  NetDeviceContainer d2 = csma.Install (net2);
  csma.SetDeviceAttribute ("Mtu", UintegerValue (150));
  NetDeviceContainer d1 = csma.Install (net1);
  NetDeviceContainer d3 = csma.Install (net01);
  NetDeviceContainer d4 = csma.Install (net02);
  NetDeviceContainer d5 = csma.Install (net03);
  NetDeviceContainer d6 = csma.Install (net04);

  SixLowPanHelper sixlowpan;
  sixlowpan.SetDeviceAttribute ("ForceEtherType", BooleanValue (true) );
  NetDeviceContainer six1 = sixlowpan.Install (d1);
   NetDeviceContainer six2 = sixlowpan.Install (d3);
   NetDeviceContainer six3 = sixlowpan.Install (d4);
  NetDeviceContainer six4 = sixlowpan.Install (d5);
   NetDeviceContainer six5 = sixlowpan.Install (d6);

  NS_LOG_INFO ("Create networks and assign IPv6 Addresses.");
  Ipv6AddressHelper ipv6;
  ipv6.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer i1 = ipv6.Assign (six1);
  i1.SetForwarding (1, true);
  i1.SetDefaultRouteInAllNodes (1);

   
  ipv6.SetBase (Ipv6Address ("2001:3::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer i3 = ipv6.Assign (six2);
  i3.SetForwarding (1, true);
  i3.SetDefaultRouteInAllNodes (1);


  ipv6.SetBase (Ipv6Address ("2001:4::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer i4 = ipv6.Assign (six3);
  i4.SetForwarding (1, true);
  i4.SetDefaultRouteInAllNodes (1);


  ipv6.SetBase (Ipv6Address ("2001:5::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer i5 = ipv6.Assign (six4);
  i5.SetForwarding (1, true);
  i5.SetDefaultRouteInAllNodes (1);

  ipv6.SetBase (Ipv6Address ("2001:6::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer i6 = ipv6.Assign (six5);
  i6.SetForwarding (1, true);
  i6.SetDefaultRouteInAllNodes (1);



    //six1 = sixlowpan.Install (d2);    
  ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer i2 = ipv6.Assign (d2);
  i2.SetForwarding (0, true);
  i2.SetDefaultRouteInAllNodes (0);

  /* Create a Ping6 application to send ICMPv6 echo request from n0 to n1 via r */
  uint32_t packetSize = 200;
  uint32_t maxPacketCount = 1000;
  Time interPacketInterval = Seconds (1.);
  Ping6Helper ping6;

  ping6.SetLocal (i1.GetAddress (0, 1));
  ping6.SetRemote (i2.GetAddress (1, 1));

  ping6.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  ping6.SetAttribute ("Interval", TimeValue (interPacketInterval));
  ping6.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer apps = ping6.Install (net1.Get (0));

  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (100.0));



  uint16_t port = 9; 

  UdpEchoClientHelper client (i2.GetAddress (1, 1), port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
   ApplicationContainer  apps1 = client.Install (n01.Get (0));
  apps1.Start (Seconds (1.0));
  apps1.Stop (Seconds (100.0));





  UdpEchoClientHelper client1 (i2.GetAddress (1, 1), port);
  client1.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client1.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client1.SetAttribute ("PacketSize", UintegerValue (packetSize));
  apps1 = client1.Install (n02.Get (0));
  apps1.Start (Seconds (10.0));
  apps1.Stop (Seconds (100.0));


 
  UdpEchoClientHelper client2 (i2.GetAddress (1, 1), port);
  client2.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client2.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client2.SetAttribute ("PacketSize", UintegerValue (packetSize));
  apps1 = client2.Install (n03.Get (0));
  apps1.Start (Seconds (20.0));
  apps1.Stop (Seconds (100.0));

 

   UdpEchoClientHelper client4 (i2.GetAddress (1, 1), port);
  client4.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client4.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client4.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer apps11 = client4.Install (n05.Get (0));
  apps11.Start (Seconds (50.0));
  apps11.Stop (Seconds (100.0));







  UdpEchoServerHelper server (port);
  ApplicationContainer apps2 = server.Install (n1.Get (0));
  apps2.Start (Seconds (1.0));
  apps2.Stop (Seconds (100.0));



  UdpEchoClientHelper client3 (i2.GetAddress (1, 1), port);
  client3.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client3.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client3.SetAttribute ("PacketSize", UintegerValue (packetSize));
   apps2 = client3.Install (r.Get (0));
   apps2.Start (Seconds (1.0));
   apps2.Stop (Seconds (100.0));





  AsciiTraceHelper ascii;
  csma.EnableAsciiAll (ascii.CreateFileStream ("6lowpan/udp-6lowpan.tr"));
 // csma.EnablePcapAll (std::string ("example-sixlowpan"), true);
  pAnim = new AnimationInterface (animFile);
 resourceId1 = pAnim->AddResource ("//home/nanho/ns-allinone-3.31/netanim-3.108/Pc.png");
 resourceId2 = pAnim->AddResource ("//home/nanho/ns-allinone-3.31/netanim-3.108/EB.png");
  resourceId3 = pAnim->AddResource ("//home/nanho/ns-allinone-3.31/netanim-3.108/Leaf.svg");
  resourceId4 = pAnim->AddResource ("//home/nanho/ns-allinone-3.31/netanim-3.108/Server.png");
  pAnim->SetBackgroundImage ("//home/nanho/ns-allinone-3.31/netanim-3.108/ns-3-background.png", 0, 0, 0.2, 0.2, 0.1);

   pAnim->UpdateNodeImage (0 , resourceId1);
   pAnim->UpdateNodeImage (1 , resourceId1);
   pAnim->UpdateNodeImage (2 , resourceId1);
   pAnim->UpdateNodeImage (3 , resourceId1);
   pAnim->UpdateNodeImage (4 , resourceId1);
   pAnim->UpdateNodeImage (5 , resourceId1);

   
  pAnim->UpdateNodeSize (0, 2, 2);
  pAnim->UpdateNodeSize (1, 2, 2);
  pAnim->UpdateNodeSize (2, 2, 2);
  pAnim->UpdateNodeSize (3, 2, 2);
  pAnim->UpdateNodeSize (4, 0, 0);
  pAnim->UpdateNodeSize (5, 2, 2);

 pAnim->UpdateNodeImage (6, resourceId3);
   pAnim->UpdateNodeImage (7, resourceId4);
 pAnim->UpdateNodeSize (6, 5, 5);
 pAnim->UpdateNodeSize (7, 5, 5);



  Simulator::Stop (Seconds (100));
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
delete pAnim; 
}

