# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

from . import infinity_brpc_pb2 as infinity__brpc__pb2


class InfinityBrpcServiceStub(object):
    """network
    """

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.Connect = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/Connect',
                request_serializer=infinity__brpc__pb2.Empty.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.Echo = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/Echo',
                request_serializer=infinity__brpc__pb2.EchoRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.EchoResponse.FromString,
                )
        self.DisConnect = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/DisConnect',
                request_serializer=infinity__brpc__pb2.DisConnectRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.CreateDatabase = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/CreateDatabase',
                request_serializer=infinity__brpc__pb2.CreateDatabaseRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.DropDatabase = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/DropDatabase',
                request_serializer=infinity__brpc__pb2.DropDatabaseRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.ListDatabase = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/ListDatabase',
                request_serializer=infinity__brpc__pb2.ListDatabaseRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.ListDatabaseResponse.FromString,
                )
        self.DescribeDatabase = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/DescribeDatabase',
                request_serializer=infinity__brpc__pb2.DescribeDatabaseRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.DescribeDatabaseResponse.FromString,
                )
        self.GetDatabase = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/GetDatabase',
                request_serializer=infinity__brpc__pb2.GetDatabaseRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.CreateTable = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/CreateTable',
                request_serializer=infinity__brpc__pb2.CreateTableRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.DropTable = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/DropTable',
                request_serializer=infinity__brpc__pb2.DropTableRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.ListTable = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/ListTable',
                request_serializer=infinity__brpc__pb2.ListTableRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.ListTableResponse.FromString,
                )
        self.DescribeTable = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/DescribeTable',
                request_serializer=infinity__brpc__pb2.DescribeTableRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.DescribeTableResponse.FromString,
                )
        self.GetTable = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/GetTable',
                request_serializer=infinity__brpc__pb2.GetTableRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.Insert = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/Insert',
                request_serializer=infinity__brpc__pb2.InsertRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.Import = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/Import',
                request_serializer=infinity__brpc__pb2.ImportRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.CreateIndex = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/CreateIndex',
                request_serializer=infinity__brpc__pb2.CreateIndexRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.DropIndex = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/DropIndex',
                request_serializer=infinity__brpc__pb2.DropIndexRequest.SerializeToString,
                response_deserializer=infinity__brpc__pb2.CommonResponse.FromString,
                )
        self.Search = channel.unary_unary(
                '/infinity_brpc_proto.InfinityBrpcService/Search',
                request_serializer=infinity__brpc__pb2.SelectStatement.SerializeToString,
                response_deserializer=infinity__brpc__pb2.SelectResponse.FromString,
                )


class InfinityBrpcServiceServicer(object):
    """network
    """

    def Connect(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Echo(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def DisConnect(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def CreateDatabase(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def DropDatabase(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def ListDatabase(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def DescribeDatabase(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def GetDatabase(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def CreateTable(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def DropTable(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def ListTable(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def DescribeTable(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def GetTable(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Insert(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Import(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def CreateIndex(self, request, context):
        """rpc Update(Update) returns (Empty);
        rpc Delete(Delete) returns (Empty);
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def DropIndex(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def Search(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_InfinityBrpcServiceServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'Connect': grpc.unary_unary_rpc_method_handler(
                    servicer.Connect,
                    request_deserializer=infinity__brpc__pb2.Empty.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'Echo': grpc.unary_unary_rpc_method_handler(
                    servicer.Echo,
                    request_deserializer=infinity__brpc__pb2.EchoRequest.FromString,
                    response_serializer=infinity__brpc__pb2.EchoResponse.SerializeToString,
            ),
            'DisConnect': grpc.unary_unary_rpc_method_handler(
                    servicer.DisConnect,
                    request_deserializer=infinity__brpc__pb2.DisConnectRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'CreateDatabase': grpc.unary_unary_rpc_method_handler(
                    servicer.CreateDatabase,
                    request_deserializer=infinity__brpc__pb2.CreateDatabaseRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'DropDatabase': grpc.unary_unary_rpc_method_handler(
                    servicer.DropDatabase,
                    request_deserializer=infinity__brpc__pb2.DropDatabaseRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'ListDatabase': grpc.unary_unary_rpc_method_handler(
                    servicer.ListDatabase,
                    request_deserializer=infinity__brpc__pb2.ListDatabaseRequest.FromString,
                    response_serializer=infinity__brpc__pb2.ListDatabaseResponse.SerializeToString,
            ),
            'DescribeDatabase': grpc.unary_unary_rpc_method_handler(
                    servicer.DescribeDatabase,
                    request_deserializer=infinity__brpc__pb2.DescribeDatabaseRequest.FromString,
                    response_serializer=infinity__brpc__pb2.DescribeDatabaseResponse.SerializeToString,
            ),
            'GetDatabase': grpc.unary_unary_rpc_method_handler(
                    servicer.GetDatabase,
                    request_deserializer=infinity__brpc__pb2.GetDatabaseRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'CreateTable': grpc.unary_unary_rpc_method_handler(
                    servicer.CreateTable,
                    request_deserializer=infinity__brpc__pb2.CreateTableRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'DropTable': grpc.unary_unary_rpc_method_handler(
                    servicer.DropTable,
                    request_deserializer=infinity__brpc__pb2.DropTableRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'ListTable': grpc.unary_unary_rpc_method_handler(
                    servicer.ListTable,
                    request_deserializer=infinity__brpc__pb2.ListTableRequest.FromString,
                    response_serializer=infinity__brpc__pb2.ListTableResponse.SerializeToString,
            ),
            'DescribeTable': grpc.unary_unary_rpc_method_handler(
                    servicer.DescribeTable,
                    request_deserializer=infinity__brpc__pb2.DescribeTableRequest.FromString,
                    response_serializer=infinity__brpc__pb2.DescribeTableResponse.SerializeToString,
            ),
            'GetTable': grpc.unary_unary_rpc_method_handler(
                    servicer.GetTable,
                    request_deserializer=infinity__brpc__pb2.GetTableRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'Insert': grpc.unary_unary_rpc_method_handler(
                    servicer.Insert,
                    request_deserializer=infinity__brpc__pb2.InsertRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'Import': grpc.unary_unary_rpc_method_handler(
                    servicer.Import,
                    request_deserializer=infinity__brpc__pb2.ImportRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'CreateIndex': grpc.unary_unary_rpc_method_handler(
                    servicer.CreateIndex,
                    request_deserializer=infinity__brpc__pb2.CreateIndexRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'DropIndex': grpc.unary_unary_rpc_method_handler(
                    servicer.DropIndex,
                    request_deserializer=infinity__brpc__pb2.DropIndexRequest.FromString,
                    response_serializer=infinity__brpc__pb2.CommonResponse.SerializeToString,
            ),
            'Search': grpc.unary_unary_rpc_method_handler(
                    servicer.Search,
                    request_deserializer=infinity__brpc__pb2.SelectStatement.FromString,
                    response_serializer=infinity__brpc__pb2.SelectResponse.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'infinity_brpc_proto.InfinityBrpcService', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class InfinityBrpcService(object):
    """network
    """

    @staticmethod
    def Connect(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/Connect',
            infinity__brpc__pb2.Empty.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Echo(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/Echo',
            infinity__brpc__pb2.EchoRequest.SerializeToString,
            infinity__brpc__pb2.EchoResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def DisConnect(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/DisConnect',
            infinity__brpc__pb2.DisConnectRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def CreateDatabase(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/CreateDatabase',
            infinity__brpc__pb2.CreateDatabaseRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def DropDatabase(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/DropDatabase',
            infinity__brpc__pb2.DropDatabaseRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def ListDatabase(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/ListDatabase',
            infinity__brpc__pb2.ListDatabaseRequest.SerializeToString,
            infinity__brpc__pb2.ListDatabaseResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def DescribeDatabase(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/DescribeDatabase',
            infinity__brpc__pb2.DescribeDatabaseRequest.SerializeToString,
            infinity__brpc__pb2.DescribeDatabaseResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def GetDatabase(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/GetDatabase',
            infinity__brpc__pb2.GetDatabaseRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def CreateTable(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/CreateTable',
            infinity__brpc__pb2.CreateTableRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def DropTable(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/DropTable',
            infinity__brpc__pb2.DropTableRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def ListTable(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/ListTable',
            infinity__brpc__pb2.ListTableRequest.SerializeToString,
            infinity__brpc__pb2.ListTableResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def DescribeTable(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/DescribeTable',
            infinity__brpc__pb2.DescribeTableRequest.SerializeToString,
            infinity__brpc__pb2.DescribeTableResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def GetTable(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/GetTable',
            infinity__brpc__pb2.GetTableRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Insert(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/Insert',
            infinity__brpc__pb2.InsertRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Import(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/Import',
            infinity__brpc__pb2.ImportRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def CreateIndex(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/CreateIndex',
            infinity__brpc__pb2.CreateIndexRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def DropIndex(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/DropIndex',
            infinity__brpc__pb2.DropIndexRequest.SerializeToString,
            infinity__brpc__pb2.CommonResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def Search(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/infinity_brpc_proto.InfinityBrpcService/Search',
            infinity__brpc__pb2.SelectStatement.SerializeToString,
            infinity__brpc__pb2.SelectResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
