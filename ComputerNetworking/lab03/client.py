# _*_ coding: utf-8 _*_


import grpc
import receiver_pb2
import receiver_pb2_grpc
from google.protobuf import struct_pb2




def run():
    channel = grpc.insecure_channel('localhost:50051')
    stub = receiver_pb2_grpc.ReceiverStub(channel)


    # 自定义struct结构
    struct = struct_pb2.Struct()
    struct['idfa'] = 'idfa1'
    struct['amount'] = 123


    response = stub.receive(receiver_pb2.Event(xwhat='install', appid='fuckgod', xwhen=123, xwho='jerry', xcontext=struct))
    print("client status: %s received: %s" % (response.status, response.message))




if __name__ == '__main__':
    run()