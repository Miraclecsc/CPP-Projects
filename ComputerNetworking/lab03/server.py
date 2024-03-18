# _*_ coding: utf-8 _*_


import grpc
import receiver_pb2
import receiver_pb2_grpc


import time
from concurrent import futures


_ONE_DAY_IN_SECONDS = 60 * 60 * 24




class Receiver(receiver_pb2_grpc.ReceiverServicer):


    # 重写父类方法，返回消息
    def receive(self, request, context):
        print('request:', request)
        return receiver_pb2.Reply(message='Hello, %s!' % request.xwho)




if __name__ == '__main__':
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    receiver_pb2_grpc.add_ReceiverServicer_to_server(Receiver(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    print('server start...')
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        server.stop(0)