#include "../includes/Client.hpp"

//------------------------------------------------------------------------------//
//																				//
//		Code For the input handler : get the args, check them, parse them		//
//		and send them into the connnection settings struct.						//
//																				//
//------------------------------------------------------------------------------//

ClientInputHandler::ClientInputHandler(int argc, char **argv)
{
	if (argc != 5 && argc != 7)
	{
		throw (CustomException("Incorrect number of args"));
	}
	else
	{
		// These method will throw exceptions if there's a problem.
		CheckInputFormat(argc, argv);
		ParseInput(argc, argv);
		FillInput(argc, argv);
	}
}

ClientInputHandler::~ClientInputHandler()
{}

void	ClientInputHandler::CheckInputFormat(int argc, char **argv)
{
	if (strncmp(argv[1], "-n", 2) != 0)
	{
		throw (CustomException("Argument 1 (TeamName specifier -n) incorrect"));
	}
	if (std::regex_match (argv[2], std::regex("^\\w+")) == false)
	{
		throw (CustomException("Argument 2 (TeamName string) incorrect"));
	}
	if (strncmp(argv[3], "-p", 2) != 0)
	{
		throw (CustomException("Argument 3 (Port specifier -p) incorrect"));
	}
	if (std::regex_match (argv[4], std::regex("^\\d+")) == false)
	{
		throw (CustomException("Argument 4 (Port number) incorrect"));
	}

	// check optional hostname;
	if (argc == 7)
	{
		if (strncmp(argv[5], "-h", 2) != 0)
		{
			throw (CustomException("Argument 5 (HostName specifier -h) incorrect"));
		}
		if (std::regex_match (argv[6], 
			std::regex("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$"))
						 == false)
		{
			throw (CustomException("Argument 6 (HostName string) incorrect"));
		}
	}
}

void	ClientInputHandler::ParseInput(int argc, char **argv)
{
	// Check team name.
	if (strlen(argv[2]) > 40)
	{
		throw (CustomException("Team Name is too long (40 char max)"));
	}

	// Check port number.
	if (std::stoi(argv[4], NULL, 10) < 1023 || std::stoi(argv[4], NULL, 10) > 65535)
	{
		throw (CustomException("Invalid port number [1023 <=> 65535]"));
	}
}

void	ClientInputHandler::FillInput(int argc, char **argv)
{
	teamName = argv[2];
	port = std::stoi(argv[4], NULL, 10);
	if (argc == 7)
		hostName = argv[6];
	else
		hostName = "localhost";
}
