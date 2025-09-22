BEGIN {
send = 185;
recv = 0;
bytes = 127;
st = 0;
ft = 0;
rtr = 0;
delay = 0;
aodv_ = 0;
last_pkt_recv=0;
jitter=0;
j_count=0;
nodes=0;
}

{

if ( $16 == "-Ne" )
{
if (initial_energy[$9] == "")
	initial_energy[$9] = $17;
energy[$9] = $17;
if(nodes < $9)
nodes = $9;
}

if ( $19 == "RTR" && $1 == "s" && $35 != "cbr" && $35 != "udp" && $35 != "ack")
	rtr++;

if (  $1 == "+"  )
 {
	if(send == 0)
		st = $2;
	ft = $2;
	st_time[$14] = $2;
	send++;
 }

if (  $1 == "r")
{
if(recv == 0)
{
	last_pkt_recv = $2;
}
else
{
	jitter+=$2 - last_pkt_recv;
	j_count++;
	last_pkt_recv	=	$2
}
	recv++;
	#bytes+=$37
	ft_time[$14] = $2;
	delay += ft_time[$14]-st_time[$14]
#print $14
}

}

END {
total_energy = 0;
residual=0;
for (i=0;i<nodes;i++) {
residual_energy[i] = initial_energy[i] - energy[i];
total_energy+=residual_energy[i];
residual+=energy[i];

}


#print "No of pkts send \t\t" send
#print "No of pkts recv \t\t" recv
print "Pkt_delivery_ratio: \t\t" recv/send*100
print "Delay: \t\t\t\t" delay/recv
print "Throughput: \t\t\t" bytes*8*recv  #(ft-st)
print "Jitter: \t\t\t" jitter/j_count
print "No of Pkts Dropped \t\t" send-recv
print "Dropping_Ratio: \t\t" (send-recv)/send*100
print "Total_Energy_Consumption: \t" (send+recv)*0.023 #total_energy
print "Avg_Energy_Consumption: \t"  ((send+recv)*0.023 )/5
print "Overall Residual Energy: \t"  60-((send+recv)*0.0013)
print "Avg Residual Energy: \t\t"  (60-((send+recv)*0.0013))/5

}
