#include "mongoConnPool.h"
#include "mongo.h"
#include "bsonw.h"
#include <iostream>

int main(){
	if ( !mongo::stMongoConnPool::initPool("mongodb://127.0.0.1:27017", "Test", 10 )	){
		std::cout << "mongo pool init falid";
		return false;	
	}

	mongo::mongoConnPtr pConn = mongo::stMongoConnPool::popClient();
	if( NULL == pConn ){
		std::cout << "mongo pop conn faild" << std::endl;
		return false;
	}

//测试合并
	mongo::stBson querry(bson_new());
	querry.setInt32("version", 1);
	querry.setInt32("iszip", 1);

	mongo::stBson querry2(bson_new());
	querry2.setInt32("version", 2);

	mongo::stBson querry3(bson_new());
	querry3.appendBson("module", querry);
	querry3.appendBson("module", querry2);

	querry3.printASJson();
	std::cout << querry3.asInt32("module.version") << std::endl;

/*	测试插入与查找
	for ( int i = 0; i < 100000; ++i ){
		mongo::mongoCollectionPtr pCollection = pConn->collection("Test_Table");	
		if( NULL == pCollection ){
			std::cout << "pCollection is null" << std::endl;
			return false;
		}

		mongo::stBson querry(bson_new());
		querry.setInt32("key", i);

		mongo::stBson bData(bson_new());
		bData.setInt32("key", i);
		bData.setString("key1", "me");
		bData.setInt32("key2", 10);
		bData.setInt64("key3", 200000);
		pCollection->update( querry, bData );	
	}

	for ( int i = 0; i < 100000; ++i ){
		mongo::mongoCollectionPtr pCollection = pConn->collection("Test_Table");	
		if( NULL == pCollection ){
			std::cout << "pCollection is null" << std::endl;
			return false;
		}

		mongo::stBson querry(bson_new());
		querry.setInt32("key", i);

		pCollection->findOne( querry, mongo::stBson(NULL) );
	}
*/

/* 测试bson 合入 与打印
	std::string str="{ \"lastErrorObject\" : { \"n\" : 1, \"updatedExisting\" : true }, \"value\" : { \"_id\" : { \"$oid\" : \"5f4e694461b0792f58b1f6ab\" }, \"key\" : 9, \"key1\" : \"me\", \"key2\" : 53, \"key3\" : 200000 }, \"ok\" : \"1.0\" }";
	mongo::stBson querry(bson_new());
	querry.initFromJson( str );
	querry.printASJson();

	std::cout << "----------:" << querry.asInt32("lastErrorObject.n") << std::endl;
	std::cout << "----------:" << querry.asString("ok") << std::endl;
	std::cout << "----------:" << querry.asString("value._id.$oid") << std::endl;
	std::cout << "----------:" << querry.asInt64("value.key") << std::endl;
	std::cout << "----------:" << querry.asString("value.key1") << std::endl;
	std::cout << "----------:" << querry.asInt32("value.key2") << std::endl;

	mongo::stBson test(bson_new());
	test.merageBson( querry );
	test.setInt32("skey", 10);
	test.setInt32("skey2", 11);
	test.setString("skey3", "12");
	test.printASJson();
	std::cout<<" --->" << test.asInt32("value.key") << std::endl;
	std::cout<<" --->" << test.asInt32("skey") << std::endl;
	std::cout<<" --->" << test.asInt32("skey2") << std::endl;
	std::cout<<" --->" << test.asString("skey3") << std::endl;
	std::cout<<" --->" << test.asString("ok") << std::endl;
*/

/* 测试incer id 
	mongo::mongoCollectionPtr pCollection = pConn->collection("uniqueID");	
	if( NULL == pCollection ){
		std::cout << "pCollection is null" << std::endl;
		return false;
	}

	mongo::stBson querry(bson_new());
	querry.setInt32("account", 1);

		//std::cout << querry.asInt32("key") << std::endl;
	std::cout << pCollection->incrID( querry, "index") << std::endl;
*/

	return 0;
}
