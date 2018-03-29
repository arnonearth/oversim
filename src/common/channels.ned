package oversim.common;

// Ethernet channel for SimpleUnderlay
channel simple_ethernetline extends ned.DatarateChannel
{
    parameters:
        delay = 0ms;
        datarate = 10Mbps;
}

// Ethernet channel with packet loss for SimpleUnderlay
channel simple_ethernetline_lossy extends ned.DatarateChannel
{
    parameters:
        delay = 0ms;
        datarate = 10Mbps;
        ber = 1e-5;
}

// DSL channel for SimpleUnderlay
channel simple_dsl extends ned.DatarateChannel
{
    parameters:
        delay = 20ms;
        datarate = 1Mbps;
}

// DSL channel with packet loss for SimpleUnderlay
channel simple_dsl_lossy extends ned.DatarateChannel
{
    parameters:
        delay = 20ms;
        datarate = 1Mbps;
        ber = 1e-5;
}

// Fiberline channel for InetUnderlay
channel inet_fiberline extends ned.DatarateChannel
{
    parameters:
        delay = 1ms;
        datarate = 10Gbps;
}

// Ethernet channel for InetUnderlay
channel inet_ethernetline extends ned.DatarateChannel
{
    parameters:
        delay = 10ms;
        datarate = 10Mbps;
}

// Ethernet channel with packet loss for InetUnderlay
channel inet_ethernetline_lossy extends ned.DatarateChannel
{
    parameters:
        delay = 10ms;
        datarate = 10Mbps;
        ber = 1e-5;
}

// DSL channel for InetUnderlay
channel inet_dsl extends ned.DatarateChannel
{
    parameters:
        delay = 30ms;
        datarate = 1Mbps;
}

// DSL channel with packet loss for InetUnderlay
channel inet_dsl_lossy extends ned.DatarateChannel
{
    parameters:
        delay = 30ms;
        datarate = 1Mbps;
        ber = 1e-5;
}

// 56k modem channel for InetUnderlay
channel inet_dialup_56k extends ned.DatarateChannel
{
    parameters:
        delay = 220ms;
        datarate = 56Kbps;
}
channel denacast_channel extends ned.DatarateChannel
{
    parameters:
        delay = uniform(15ms,20ms);
        datarate = uniform(1.5Mbps,2Mbps);
}
channel VodUpload_channel extends ned.DatarateChannel
{
    parameters:
        delay = uniform(15ms,20ms);
        datarate = uniform(350Kbps,2Mbps);

}
channel VodDownload_channel extends ned.DatarateChannel
{
    parameters:
        delay = uniform(15ms,20ms);
        datarate = uniform(1Mbps,4Mbps);
}
channel VodSource_channel extends ned.DatarateChannel
{
    parameters:
        delay = 10ms;
        datarate = 15Mbps;
}
channel denacast_lossy extends ned.DatarateChannel
{
    parameters:
        delay = uniform(15ms,20ms);
        datarate = uniform(1.5Mbps,2Mbps);
        ber = uniform(1e-6,5e-6);
}

channel source_channel extends ned.DatarateChannel
{
    parameters:
        delay = 10ms;
        datarate = 10Mbps;
}
