#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import CPULimitedHost, OVSSwitch
from mininet.link import TCLink
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel, info
from mininet.cli import CLI
import time

class CustomTopo(Topo):
    def build(self):
        # 创建交换机
        switch = self.addSwitch('s1')

        # 创建服务器主机
        server = self.addHost('server', cpu=0.5, ip='10.0.0.1/24')

        # 创建客户端主机
        clientCount = 10
        clients = []
        for i in range(clientCount):
            client = self.addHost(f'client{i + 1}', cpu=0.5, ip=f'10.0.0.{i + 2}/24')
            clients.append(client)

        # 添加链路
        serverBw = 10
        clientBw = serverBw / clientCount  # 平均分配带宽给每个客户端
        self.addLink(server, switch, bw=serverBw, delay='10ms', loss=1, use_htb=True)
        for client in clients:
            self.addLink(client, switch, bw=clientBw, delay='20ms', loss=2, use_htb=True)

def create_network():
    "Create network and run simple performance test"
    topo = CustomTopo()
    net = Mininet(topo=topo, host=CPULimitedHost, link=TCLink, autoStaticArp=True)
    net.start()

    server = net.get('server')
    server.cmd('cd /root/lab01 && ./server &')
    for i in range(10):
        client = net.get(f'client{i + 1}')
        client.cmd(f'cd /root/lab01 && ./client client{i + 1} &')

    CLI(net)
    net.stop()

if __name__ == '__main__':
    setLogLevel('info')
    create_network()
