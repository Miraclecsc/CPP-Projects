# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: receiver.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from google.protobuf import struct_pb2 as google_dot_protobuf_dot_struct__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0ereceiver.proto\x1a\x1cgoogle/protobuf/struct.proto\"m\n\x05\x45vent\x12\r\n\x05\x61ppid\x18\x01 \x01(\t\x12\r\n\x05xwhen\x18\x02 \x01(\x05\x12\x0c\n\x04xwho\x18\x03 \x01(\t\x12\r\n\x05xwhat\x18\x04 \x01(\t\x12)\n\x08xcontext\x18\x05 \x01(\x0b\x32\x17.google.protobuf.Struct\"(\n\x05Reply\x12\x0e\n\x06status\x18\x01 \x01(\x05\x12\x0f\n\x07message\x18\x02 \x01(\t2\'\n\x08Receiver\x12\x1b\n\x07receive\x12\x06.Event\x1a\x06.Reply\"\x00\x62\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'receiver_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
  DESCRIPTOR._options = None
  _globals['_EVENT']._serialized_start=48
  _globals['_EVENT']._serialized_end=157
  _globals['_REPLY']._serialized_start=159
  _globals['_REPLY']._serialized_end=199
  _globals['_RECEIVER']._serialized_start=201
  _globals['_RECEIVER']._serialized_end=240
# @@protoc_insertion_point(module_scope)
