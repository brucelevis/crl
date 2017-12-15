#include <exception>
#include <sstream>
#include <strings.h>

#include "logger.h"
#include "app.h"

int main(int argc, char** argv)
{
	Application app;
	int result = 0;

	try 
	{
		for(int i = 0; i < argc; ++i)
		{
			if(strcasecmp(argv[i], "-llevel") == 0)
			{
				std::stringstream ss(argv[++i]);

				uint64_t value;
				if((ss >> value).fail() || !(ss >> std::ws).eof())
				{
					ss.clear();
					ss << "bad argument for -llevel: '" << argv[i] << "'";
					throw std::runtime_error(ss.str().c_str());
				}
				else
				{
					Logger::Instance()->setLogLevel(static_cast<Logger::Level>(value));
				}
			}
		}
	
		result = app.run();
	}
	catch(std::runtime_error& e)
	{
		std::stringstream sstream;
		sstream << "fatal runtime exception: " << e.what();

		Logger::Instance()->logLine(Logger::Level::LCRITICAL, sstream.str());

		return 1;
	}
	catch(std::exception& e)
	{
		std::stringstream sstream;
		sstream << "fatal exception: " << e.what();

		Logger::Instance()->logLine(Logger::Level::LCRITICAL, sstream.str());

		return 1;
	}
	
	return result;
}
