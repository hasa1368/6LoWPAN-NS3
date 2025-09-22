
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
  internetv6.Install (n01);
  internetv6.Install (n02);
  internetv6.Install (n03);
  internetv6.Install (n05);
  

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
   
  

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  ipv4.Assign (six1);


  Ipv4AddressHelper ipv41;
  ipv41.SetBase ("10.1.2.0", "255.255.255.0");
  ipv41.Assign (six2);


  Ipv4AddressHelper ipv42;
  ipv42.SetBase ("10.1.3.0", "255.255.255.0");
  ipv42.Assign (six3);

  Ipv4AddressHelper ipv43;
  ipv43.SetBase ("10.1.4.0", "255.255.255.0");
  ipv43.Assign (six4);

  Ipv4AddressHelper ipv44;
  ipv44.SetBase ("10.1.5.0", "255.255.255.0");
  ipv44.Assign (six5);

 

   Ipv4AddressHelper ipv45;
  //six1 = sixlowpan.Install (d2);    
  ipv45.SetBase ("10.1.6.0", "255.255.255.0");
  ipv45.Assign (d2);


   

  /* Create a Ping6 application to send ICMPv6 echo request from n0 to n1 via r */
 // uint32_t packetSize = 200;
 // uint32_t maxPacketCount = 1000;
  Time interPacketInterval = Seconds (1.);
 // Ping6Helper ping6;

   


  uint16_t port = 50000; 

Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
 Address sinkLocalAddress(InetSocketAddress (Ipv4Address ("10.1.5.2"), port));
 OnOffHelper onoff ("ns3::TcpSocketFactory",sinkLocalAddress);
  onoff.SetConstantRate (DataRate ("512Kb/s"));
  onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  ApplicationContainer app = onoff.Install (n01.Get (0));
  // Start the application
  app.Start (Seconds (1.0));
  app.Stop (Seconds (5.0));
 app.Start (Seconds (5.0));
  app.Stop (Seconds (10.0));
app.Start (Seconds (10.0));
  app.Stop (Seconds (15.0));
app.Start (Seconds (15.0));
  app.Stop (Seconds (20.0));


  OnOffHelper onoff1 ("ns3::TcpSocketFactory",Address (InetSocketAddress ( Ipv4Address ("10.1.4.2"), port)));
  
  onoff1.SetConstantRate (DataRate ("512Kb/s"));
  onoff1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
   ApplicationContainer app1 = onoff1.Install (n05.Get (0));
  // Start the application
  app1.Start (Seconds (1.0));
  app1.Stop (Seconds (100.0));
   


  OnOffHelper onoff6 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.6.2"), port)));
  //onoff6.SetAttribute ("Remote", AddressValue (InetSocketAddress (Ipv4Address ("10.1.6.2"), port)));
  onoff6.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff6.SetConstantRate (DataRate ("512Kb/s"));
  app = onoff6.Install (r.Get (0));
  app.Start (Seconds (1.1));
  app.Stop (Seconds (100.0));


  OnOffHelper onoff9 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.6.2"), port)));
   onoff9.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff9.SetConstantRate (DataRate ("512Kb/s"));
  ApplicationContainer app4 = onoff9.Install (net03.Get (0));
  app4.Start (Seconds (1.1));
  app4.Stop (Seconds (100.0));

  OnOffHelper onoff10 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.4.3"), port)));
   onoff10.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff10.SetConstantRate (DataRate ("512Kb/s"));
  ApplicationContainer app5 = onoff10.Install (net02.Get (0));
  app5.Start (Seconds (1.1));
  app5.Stop (Seconds (100.0));




  



  OnOffHelper onoff41 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.1.3"), port)));
  onoff41.SetAttribute ("Remote", AddressValue (InetSocketAddress (Ipv4Address ("10.1.1.1"), port)));
  onoff41.SetConstantRate (DataRate ("512Kb/s"));
  app = onoff41.Install (n0.Get (0));
  app.Start (Seconds (4.1));
  app.Stop (Seconds (80.0));


   OnOffHelper onoff5 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.2.2"), port)));
  onoff5.SetAttribute ("Remote", AddressValue (InetSocketAddress (Ipv4Address ("10.1.2.2"), port)));
  onoff5.SetConstantRate (DataRate ("512Kb/s"));
  app = onoff5.Install (n05.Get (0));
  app.Start (Seconds (1.1));
  app.Stop (Seconds (100.0));



  OnOffHelper onoff61 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.2.1"), port)));
  onoff61.SetAttribute ("Remote", AddressValue (InetSocketAddress (Ipv4Address ("10.1.2.1"), port)));
  onoff61.SetConstantRate (DataRate ("512Kb/s"));
  app = onoff61.Install (n01.Get (0));
  app.Start (Seconds (1.1));
  app.Stop (Seconds (100.0));


    OnOffHelper onoff7 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.2.1"), port)));
  onoff7.SetConstantRate (DataRate ("512Kb/s"));
  onoff7.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  ApplicationContainer app2 = onoff7.Install (n01.Get (0));
  // Start the application
  app2.Start (Seconds (1.0));
  app2.Stop (Seconds (100.0));


  OnOffHelper onoff8 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.1.4"), port)));
  onoff8.SetConstantRate (DataRate ("512Kb/s"));
  onoff8.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  ApplicationContainer app3 = onoff8.Install (n02.Get (0));
  // Start the application
  app3.Start (Seconds (1.0));
  app3.Stop (Seconds (100.0));

 OnOffHelper onoff91 ("ns3::TcpSocketFactory",Address (InetSocketAddress (Ipv4Address ("10.1.2.2"), port)));
  onoff91.SetAttribute ("Remote", AddressValue (InetSocketAddress (Ipv4Address ("10.1.2.2"), port)));
  onoff91.SetConstantRate (DataRate ("512Kb/s"));
 ApplicationContainer app41 = onoff91.Install (n0.Get (0));
  app41.Start (Seconds (1.1));
  app41.Stop (Seconds (100.0));














  app = onoff.Install (r.Get (0));
  app.Start (Seconds (10.1));
  app.Stop (Seconds (100.0));

  app = onoff.Install (n0.Get (0));
  app.Start (Seconds (30.1));
  app.Stop (Seconds (35.0));

  app = onoff.Install (n01.Get (0));
  app.Start (Seconds (35.1));
  app.Stop (Seconds (40.0));

  app4 = onoff.Install (n02.Get (0));
  app4.Start (Seconds (40.1));
  app4.Stop (Seconds (45.0));
  app4 = onoff.Install (n0.Get (0));
  app4.Start (Seconds (45.1));
  app4.Stop (Seconds (50.0));
   app = onoff.Install (n01.Get (0));
  app.Start (Seconds (50.1));
  app.Stop (Seconds (55.0));
  app = onoff.Install (n05.Get (0));
  app.Start (Seconds (55.1));
  app.Stop (Seconds (60.0));
  app = onoff.Install (n0.Get (0));
  app.Start (Seconds (60.1));
  app.Stop (Seconds (65.0));

  app = onoff10.Install (n01.Get (0));
  app.Start (Seconds (65.1));
  app.Stop (Seconds (70.0));

  app = onoff10.Install (n05.Get (0));
  app.Start (Seconds (70.1));
  app.Stop (Seconds (74.0));

  app = onoff10.Install (n02.Get (0));
  app.Start (Seconds (74.1));
  app.Stop (Seconds (79.0)); 
  
  app = onoff.Install (n0.Get (0));
  app.Start (Seconds (30.1));
  app.Stop (Seconds (45.0));

  app4 = onoff.Install (n0.Get (0));
  app4.Start (Seconds (45.1));
  app4.Stop (Seconds (55.0));








  AsciiTraceHelper ascii;
  csma.EnableAsciiAll (ascii.CreateFileStream ("6lowpan/tcp-6lowpan.tr"));
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

