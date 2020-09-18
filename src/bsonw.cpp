#include "bsonw.h"

#include <string.h>

namespace mongo {

stBson::stBson(bson_t* t) : mBson(t) {
}

stBson::~stBson(){
    if ( mBson ){
        bson_destroy(mBson);
    }
}

void stBson::printASJson(){
	printf("jsonData:[%s]\n", bson_as_json( mBson, NULL ) );
}

//追加bson
bool stBson::appendBson( const std::string& key, const stBson& bData ){
    if ( !bData.mBson ){
        printf("data bson is null");
        return false;
    }
    bson_append_document( mBson, key.c_str(), (int)key.length(), bData.mBson );
    return true;
}

bool stBson::initFromJson( const std::string &jsonData ){
	if ( jsonData.empty() ){
		printf("bson init string is null");
		return false;
	}

	if ( !mBson ){
		printf("bson is null");
		return false;
	}

	bson_error_t err;	
	if ( !bson_init_from_json ( mBson, jsonData.c_str(), jsonData.size(), &err) ){
		printf( "bson[%s] init faild\n", jsonData.c_str() );
		return false;
	}
	return true;
}

//合并bson
bool stBson::merageBson( const stBson &bData ){
    if( !bData.mBson ){
        printf("data bson is null");
        return false;
    }

    bson_iter_t iter;
    if ( !bson_iter_init( &iter, bData.mBson ) ) {
        return false;
    }

    while (bson_iter_next(&iter)) {
        bson_append_iter(mBson, bson_iter_key(&iter), bson_iter_key_len(&iter), &iter);
    }
    return true;
}

bool stBson::hasKey( const std::string &dotKey ){
    bson_iter_t iterD, iterK;
    if ( !mBson
	    || !bson_iter_init(&iterD, mBson)
		|| !bson_iter_find_descendant(&iterD, dotKey.c_str(), &iterK )
    ){
        return false;
    }
    return true;
}

bson_type_t stBson::getValueType( const std::string& dotKey ){
    bson_iter_t iterD, iterK;
    if (!mBson
	    || !bson_iter_init(&iterD, mBson)
		|| !bson_iter_find_descendant(&iterD, dotKey.c_str(), &iterK )
    ){
	    return BSON_TYPE_EOD;
    }
    return bson_iter_type(&iterK);
}

//判断是否是文档类型
bool stBson::isObject( const std::string&  dotKey ){
    bson_type_t t = getValueType(dotKey);
    return  t == BSON_TYPE_DOCUMENT || t == BSON_TYPE_NULL || t == BSON_TYPE_UNDEFINED;
}

BsonPtr stBson::asObject( const std::string&  dotKey ){
    bson_iter_t iterD, iterK;
    if ( !mBson
        || !bson_iter_init(&iterD, mBson)
        || !bson_iter_find_descendant(&iterD, dotKey.c_str(), &iterK)
    ) {
        printf("Bson::init error");
        return NULL;
    }

    auto t = bson_iter_type( &iterK );
    if ( t != BSON_TYPE_DOCUMENT ) {
        printf("Bson::asObject error");
        return NULL;
    }

    const uint8_t *buf = NULL;
    uint32_t len = 0;
    bson_iter_document(&iterK, &len, &buf);
    return BsonPtr(new stBson(bson_new_from_data(buf, len)));
}

//判断是否是字符串  
bool stBson::isString( const std::string&  dotKey ){
    return getValueType(dotKey) == BSON_TYPE_UTF8;    
}

std::string stBson::asString( const std::string& dotKey){
    bson_iter_t iterK, iterD;
    if ( !mBson 
        || !bson_iter_init(&iterD, mBson)
	    || !bson_iter_find_descendant(&iterD, dotKey.c_str(), &iterK)
        || !BSON_ITER_HOLDS_UTF8(&iterK)) {
		printf("key[%s] as String faild", dotKey.c_str());
        return "";
	}
	return bson_iter_utf8(&iterK, nullptr);
}

//判断是否是uint64
bool stBson::isInt64( const std::string&  dotKey ){
    return getValueType(dotKey) == BSON_TYPE_INT64;    
}

int64_t stBson::asInt64( const std::string&  dotKey ){
    bson_iter_t iterD, iterK;
    if ( !mBson
        || !bson_iter_init(&iterD, mBson)
		|| !bson_iter_find_descendant(&iterD, dotKey.c_str(), &iterK) 
        || ( !BSON_ITER_HOLDS_INT64(&iterK) && !BSON_ITER_HOLDS_INT32(&iterK) )
    ) {
		printf("key[%s] as init64 faild", dotKey.c_str());
	    return 0; 
	}
    return bson_iter_as_int64(&iterK);
}

//判断是否是uint32
bool stBson::isInt32( const std::string&  dotKey ){
    return getValueType(dotKey) == BSON_TYPE_INT64;    
}

int32_t stBson::asInt32( const std::string& dotKey ){
    bson_iter_t iterD, iterK;
    if ( !mBson
        || !bson_iter_init(&iterD, mBson)
		|| !bson_iter_find_descendant(&iterD, dotKey.c_str(), &iterK) 
        || !BSON_ITER_HOLDS_INT32(&iterK)
    ) {
		printf("key[%s] as init32 faild", dotKey.c_str());
	    return 0; 
	}
    return bson_iter_as_int64(&iterK);
}

//判断是否是Double
bool stBson::isDouble( const std::string&  dotKey ){
    return getValueType(dotKey) == BSON_TYPE_INT64;    
}

uint32_t stBson::asDouble( const std::string& dotKey ){
    bson_iter_t iterD, iterK;
    if ( !mBson
        || !bson_iter_init(&iterD, mBson)
	    || !bson_iter_find_descendant(&iterD, dotKey.c_str(), &iterK)
        || !BSON_ITER_HOLDS_NUMBER(&iterK)
    ) {
		printf("key[%s] as Double faild", dotKey.c_str());
		return 0.0;
    }
	return bson_iter_as_double(&iterK);
}

//是否是binary
bool stBson::isBinary( const std::string&  dotKey ){
    return getValueType(dotKey) == BSON_TYPE_BINARY;    
}

bool stBson::asBinary( const std::string& dotKey, uint8_t *outData, uint32_t outLen ){
    bson_iter_t iterD, iterK;
    const uint8_t *result = NULL;
    uint32_t length = 0;
    if ( !mBson
        || !bson_iter_init(&iterD, mBson)
	    || !bson_iter_find_descendant(&iterD, dotKey.c_str(), &iterK)
        || !BSON_ITER_HOLDS_BINARY(&iterK)
    ) {
		printf("key[%s] as Binary faild", dotKey.c_str());
        return false;
    }
    bson_subtype_t subType = BSON_SUBTYPE_BINARY;
    bson_iter_binary(&iterK, &subType, &length, &result);

    if(  length > outLen ){
        printf("key[%s] outSize[%u] length not equal real size[%u]" , dotKey.c_str(), outLen, length);
        return false;
    }
    memcpy( outData, result, length ); 
    return true;
}

bool stBson::setString( const std::string&  key, const std::string& value ){
    return BSON_APPEND_UTF8( mBson, key.c_str() , value.c_str() );
}

bool stBson::setInt32( const std::string& key, int32_t value ){
    return BSON_APPEND_INT32( mBson, key.c_str(), value );
}

bool stBson::setInt64( const std::string& key, int64_t value  ){
    return BSON_APPEND_INT64( mBson, key.c_str(), value );
}

bool stBson::setBinary( const std::string& key,  const uint8_t *value, uint32_t length ){
    return BSON_APPEND_BINARY( mBson, key.c_str(), BSON_SUBTYPE_BINARY, value, length );
}

}

