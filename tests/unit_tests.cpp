//
// Created by Christopher Schmidt on 28.07.15.
//

#include "unit_tests.h"


BOOST_AUTO_TEST_CASE( ConfigHandler_FaultyFiles ) {
    ConfigHandler *conf = ConfigHandler::getInstance();

    BOOST_REQUIRE(not conf->init("tests/test_fail.json"));
    BOOST_REQUIRE(not conf->init("tests/nonexistent.json"));
}

BOOST_AUTO_TEST_CASE( ConfigHandler_FileOperations ) {
    boost::filesystem::path example("tests/test.json");
    boost::filesystem::path file("tests/temp.json");
    boost::filesystem::copy_file(example, file);


    ConfigHandler *conf = ConfigHandler::getInstance();
    BOOST_REQUIRE(conf->init("tests/temp.json"));
    BOOST_CHECK_EQUAL(conf->config["key"], "value");

    boost::filesystem::remove(file);
}

BOOST_AUTO_TEST_CASE( HttpDaemon_withoutSSL ) {
    json config = "{\"port\": 8888,\"ssl\": false}"_json;
    Http::Daemon daemon;
    BOOST_REQUIRE(daemon.init(config));
}

BOOST_AUTO_TEST_CASE( HttpDaemon_ConfigTest ) {
    Http::Daemon d;

    json config_correct1 = "{\"port\": 8889, \"ssl\": false}"_json;
    json config_correct2 = "{\"port\": 8889, \"ssl\": {\"cert\": \"server.pem\",\"key\": \"server.key\"}}"_json;
    json config_incorrect1 = "{\"port\": 8889, \"ssl\": {\"cert\": 1,\"key\": \"server.key\"}}"_json;
    json config_incorrect2 = "{\"port\": 8889, \"ssl\": {\"cert\": \"server.pem\",\"key\": 1}}"_json;
    json config_incorrect3 = "{\"port\": 8889, \"ssl\": {\"key\": \"server.key\"}}"_json;
    json config_incorrect4 = "{\"port\": 8889, \"ssl\": {\"cert\": \"server.pem\"}}"_json;
    json config_incorrect5 = "{\"port\": 8889, \"ssl\": {\"cert\": \"notexistent.pem\",\"key\": \"404.pem\"}}"_json;
    json config_incorrect6 = "{\"port\": 8889, \"ssl\": true}"_json;
    json config_incorrect7 = "{\"ssl\": false}"_json;
    json config_incorrect8 = "{\"port\": \"string\", \"ssl\": false}"_json;

    BOOST_REQUIRE(d.init(config_correct1));
    BOOST_REQUIRE(d.init(config_correct2));
    BOOST_REQUIRE(not d.init(config_incorrect1));
    BOOST_REQUIRE(not d.init(config_incorrect2));
    BOOST_REQUIRE(not d.init(config_incorrect3));
    BOOST_REQUIRE(not d.init(config_incorrect4));
    BOOST_REQUIRE(not d.init(config_incorrect5));
    BOOST_REQUIRE(not d.init(config_incorrect6));
    BOOST_REQUIRE(not d.init(config_incorrect7));
    BOOST_REQUIRE(not d.init(config_incorrect8));
}

BOOST_AUTO_TEST_CASE( HttpDaemon_invalidPort ) {
    json config1 = "{\"port\": -1,\"ssl\": false}"_json;
    Http::Daemon daemon1;
    BOOST_REQUIRE(not daemon1.init(config1));

    json config2 = "{\"port\": 70000,\"ssl\": false}"_json;
    Http::Daemon daemon2;
    BOOST_REQUIRE(not daemon2.init(config2));
}

BOOST_AUTO_TEST_CASE( HttpResponse_init ) {
    Http::Response r;
    std::string str1("test content1");
    std::string str2("test content with different length2");

    BOOST_REQUIRE(not r.is_static());

    r.setContent(str1);
    BOOST_REQUIRE_EQUAL( r.getRawDataSize(), str1.size());
    BOOST_REQUIRE_EQUAL( *r.getRawData(), *str1.c_str());

    r.setContent(str2.c_str(), str2.size());
    BOOST_REQUIRE_EQUAL( r.getRawDataSize(), str2.size());
    BOOST_REQUIRE_EQUAL( *r.getRawData(), *str2.c_str());

}