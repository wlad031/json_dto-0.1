/*
	A sample using json_dto
*/

#include <iostream>
#include <string>
#include <ctime>

#include <json_dto/pub.hpp>

// Message.
struct message_t
{
	message_t() {}

	message_t(
		std::string from,
		std::int64_t when,
		std::string text )
		:	m_from{ std::move( from ) }
		,	m_when{ when }
		,	m_text{ std::move( text ) }
	{}

	// Who sent a message.
	std::string m_from;
	// When the message was sent (unixtime).
	std::int64_t m_when;
	// Message text.
	std::string m_text;

	template < typename JSON_IO >
	void
	json_io( JSON_IO & io )
	{
		io
			& json_dto::mandatory( "from", m_from )
			& json_dto::mandatory( "when", m_when )
			& json_dto::mandatory( "text", m_text );
	}
};

const std::string json_data{
R"JSON({
  "from" : "json_dto",
  "when" : 1474884330,
  "text" : "Hello world!"
})JSON" };

int
main( int , char *[] )
{
	try
	{
		{
			rapidjson::Document document;
			document.Parse( json_data.c_str() );

			json_dto::json_input_t jin{ document };

			message_t msg;
			jin >> msg;

			const auto t = static_cast< std::time_t >( msg.m_when );
			std::cout
				<< "Deserialized from JSON:\n"
				<< "\tfrom: " << msg.m_from << "\n"
				<< "\twhen: " << std::ctime( &t )
				<< "\ttext: " << msg.m_text << std::endl;
		}

		{
			const message_t msg{ "json_dto", std::time( nullptr ), "Hello once again!" };

			rapidjson::Document output_doc;
			json_dto::json_output_t jout{
				output_doc, output_doc.GetAllocator() };

			jout << msg;

			rapidjson::StringBuffer buffer;
			rapidjson::Writer< rapidjson::StringBuffer > writer( buffer );
			output_doc.Accept( writer );

			std::cout
				<< "\nSerialized to JSON:\n"
				<< buffer.GetString() << std::endl;
		}
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << "." << std::endl;
		return 1;
	}

	return 0;
}
