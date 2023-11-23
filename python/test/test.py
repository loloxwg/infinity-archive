# Copyright(C) 2023 InfiniFlow, Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
from sqlglot import condition

import infinity
from infinity.infinity import NetworkAddress
from infinity.remote_thrift.table import traverse_conditions

class TestCase:

    def test_version(self):
        print(infinity.__version__)

    def test_connection(self):
        """
        target: test connect and disconnect server ok
        method: connect server
        expect: connect and disconnect successfully
        """
        infinity_obj = infinity.connect(NetworkAddress('0.0.0.0', 9090))
        assert infinity_obj
        assert infinity_obj.disconnect()

    def test_create_db_with_invalid_name(self):
        """
        target: test db name limitation
        method: create db with empty name
        expect: create db fail with error message
        """
        infinity_obj = infinity.connect(NetworkAddress('0.0.0.0', 9090))
        assert infinity_obj

        res = infinity_obj.create_database("")
        assert res.success == False
        assert res.error_msg

        assert infinity_obj.disconnect()

    def test_infinity_thrift(self):
        """
        target: test basic operation
        method:
        1. connect
        2. create db
        3. list db
        4. drop db
        5. get db
        6. create table
        7. drop table
        8. create index
        9. drop index
        10. insert and search
        11. import and search
        12.
        expect: all operations successfully
        """
        infinity_obj = infinity.connect(NetworkAddress('0.0.0.0', 9090))
        assert infinity_obj

        # infinity
        res = infinity_obj.create_database("my_db")
        assert res.success

        res = infinity_obj.list_databases()
        assert res.success

        for db in res.db_names:
            assert db in ['my_db', 'default']

        res = infinity_obj.drop_database("my_db")
        assert res.success

        db_obj = infinity_obj.get_database("default")
        res = db_obj.create_table("my_table1", {"c1": "int, primary key"}, None)
        assert res.success
        
        res = db_obj.list_tables()
        assert res.success

        res = db_obj.drop_table("my_table1")
        assert res.success


        # index
        res = db_obj.create_table("my_table2", {"c1": "vector,1024,float"}, None)
        assert res.success
        table_obj = db_obj.get_table("my_table2")
        assert table_obj

        res = table_obj.create_index("my_index", ["c1"], "IVFFlat", [{"centroids_count": 128}, {"metric": "l2"}], None)
        assert res.success

        res = table_obj.drop_index("my_index")
        assert res.success

        res = db_obj.drop_table("my_table2")
        assert res.success

        # insert
        res = db_obj.create_table("my_table3", {"c1": "int, primary key", "c2": "float"}, None)
        assert res.success

        table_obj = db_obj.get_table("my_table3")
        assert table_obj

        res = table_obj.insert([{"c1": 1, "c2": 1.1}, {"c1": 2, "c2": 2.2}])
        assert res.success
        # search
        res = table_obj.search().output(["c1", "c2"]).filter("c1 > 1").to_list()
        print(res)

        res = db_obj.drop_table("my_table3")
        assert res.success

        # import
        res = db_obj.create_table("my_table4", {"c1": "int", "c2": "vector,3,int"}, None)
        assert res.success
        table_obj = db_obj.get_table("my_table4")
        assert table_obj

        parent_dir = os.path.dirname(os.path.dirname(os.getcwd()))
        test_csv_dir = parent_dir + "/test/data/csv/embedding_int_dim3.csv"
        assert os.path.exists(test_csv_dir)

        res = table_obj.import_data(test_csv_dir, None)
        assert res.success

        # search
        res = table_obj.search().output(["c1"]).filter("c1 > 1").to_list()
        print(res)
        res = db_obj.drop_table("my_table4")
        assert res.success

        # disconnect
        res = infinity_obj.disconnect()
        assert res.success

    def test_infinity_grpc(self):
        infinity_obj = infinity.connect(NetworkAddress('0.0.0.0', 50052))
        assert infinity_obj

        # infinity
        res = infinity_obj.create_database("my_db")
        assert res.success

        res = infinity_obj.list_databases()
        assert res.success

        for db in res.db_names:
            assert db in ['my_db', 'default']

        res = infinity_obj.drop_database("my_db")
        assert res.success

        db_obj = infinity_obj.get_database("default")
        res = db_obj.create_table("my_table1", {"c1": "int, primary key"}, None)
        assert res.success

        res = db_obj.list_tables()
        assert res.success

        res = db_obj.drop_table("my_table1")
        assert res.success


        # index
        res = db_obj.create_table("my_table2", {"c1": "vector,1024,float"}, None)
        assert res.success
        table_obj = db_obj.get_table("my_table2")
        assert table_obj

        res = table_obj.create_index("my_index", ["c1"], "IVFFlat", [{"centroids_count": 128}, {"metric": "l2"}], None)
        assert res.success

        res = table_obj.drop_index("my_index")
        assert res.success

        res = db_obj.drop_table("my_table2")
        assert res.success

        # insert
        res = db_obj.create_table("my_table3", {"c1": "int, primary key", "c2": "float"}, None)
        assert res.success

        table_obj = db_obj.get_table("my_table3")
        assert table_obj

        res = table_obj.insert([{"c1": 1, "c2": 1.1}, {"c1": 2, "c2": 2.2}])
        assert res.success
        # search
        res = table_obj.search().output(["c1", "c2"]).filter("c1 > 1").to_list()
        print(res)

        res = db_obj.drop_table("my_table3")
        assert res.success

        # import
        res = db_obj.create_table("my_table4", {"c1": "int", "c2": "vector,3,int"}, None)
        assert res.success
        table_obj = db_obj.get_table("my_table4")
        assert table_obj

        parent_dir = os.path.dirname(os.path.dirname(os.getcwd()))
        test_csv_dir = parent_dir + "/test/data/csv/embedding_int_dim3.csv"
        assert os.path.exists(test_csv_dir)

        res = table_obj.import_data(test_csv_dir, None)
        assert res.success

        # search
        res = table_obj.search().output(["c1"]).filter("c1 > 1").to_list()
        print(res)
        res = db_obj.drop_table("my_table4")
        assert res.success

        # disconnect
        res = infinity_obj.disconnect()
        assert res.success

    def test_infinity_brpc(self):
        infinity_obj = infinity.connect(NetworkAddress('0.0.0.0', 50051))
        assert infinity_obj

        # infinity
        res = infinity_obj.create_database("my_db")
        assert res.success

        res = infinity_obj.list_databases()
        assert res.success

        for db in res.db_names:
            assert db in ['my_db', 'default']

        res = infinity_obj.drop_database("my_db")
        assert res.success

        db_obj = infinity_obj.get_database("default")
        res = db_obj.create_table("my_table1", {"c1": "int, primary key"}, None)

        assert res.success
        res = db_obj.list_tables()
        assert res.success

        res = db_obj.drop_table("my_table1")
        assert res.success


        # index
        res = db_obj.create_table("my_table2", {"c1": "vector,1024,float"}, None)
        assert res.success
        table_obj = db_obj.get_table("my_table2")
        assert table_obj

        res = table_obj.create_index("my_index", ["c1"], "IVFFlat", [{"centroids_count": 128}, {"metric": "l2"}], None)
        assert res.success

        res = table_obj.drop_index("my_index")
        assert res.success

        res = db_obj.drop_table("my_table2")
        assert res.success

        # insert
        res = db_obj.create_table("my_table3", {"c1": "int, primary key", "c2": "float"}, None)
        assert res.success

        table_obj = db_obj.get_table("my_table3")
        assert table_obj

        res = table_obj.insert([{"c1": 1, "c2": 1.1}, {"c1": 2, "c2": 2.2}])
        assert res.success
        # search
        res = table_obj.search().output(["c1", "c2"]).filter("c1 > 1").to_list()
        print(res)

        res = db_obj.drop_table("my_table3")
        assert res.success

        # import
        res = db_obj.create_table("my_table4", {"c1": "int", "c2": "vector,3,int"}, None)
        assert res.success
        table_obj = db_obj.get_table("my_table4")
        assert table_obj

        parent_dir = os.path.dirname(os.path.dirname(os.getcwd()))
        test_csv_dir = parent_dir + "/test/data/csv/embedding_int_dim3.csv"
        assert os.path.exists(test_csv_dir)

        res = table_obj.import_data(test_csv_dir, None)
        assert res.success

        # search
        res = table_obj.search().output(["c1"]).filter("c1 > 1").to_list()
        print(res)
        res = db_obj.drop_table("my_table4")
        assert res.success

        # disconnect
        res = infinity_obj.disconnect()
        assert res.success

    def test_traverse_conditions(self):
        res = traverse_conditions(condition("c1>1 and c2<2 or c3=3.3"))
        print(res)
