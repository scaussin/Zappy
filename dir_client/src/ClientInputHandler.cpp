#include "client.hpp"

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

/*
**	This method will check if the input is well format -> correct number of arguments,
**	and options are correct. We do not parse what is entered yet, we just check the format.
*/

void	ClientInputHandler::CheckInputFormat(int argc, char **argv)
{
	if (strncmp(argv[1], "-n", 2) != 0)
	{
		throw (CustomException("Argument 1 (TeamName specifier -n) incorrect"));
	}
	if (regex_match (argv[2], regex("^[\\w ]+")) == false)
	{
		throw (CustomException("Argument 2 (TeamName string) incorrect"));
	}
	if (strncmp(argv[3], "-p", 2) != 0)
	{
		throw (CustomException("Argument 3 (Port specifier -p) incorrect"));
	}
	if (regex_match (argv[4], regex("^\\d+")) == false)
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
		if (regex_match (argv[6], 
			regex("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$"))
						 == false)
		{
			throw (CustomException("Argument 6 (HostName string) incorrect"));
		}
	}
}

/*
**	This is where we parse what is entered. We check if the team name isnt too long,
**	or that the port is a correct one ... etc.
*/

void	ClientInputHandler::ParseInput(int argc, char **argv)
{
	(void)argc;
	// Check team name.
	if (strlen(argv[2]) > 40)
	{
		throw (CustomException("Team Name is too long (40 char max)"));
	}

	// Check port number.
	if (stoi(argv[4], NULL, 10) < 1023 || stoi(argv[4], NULL, 10) > 65535)
	{
		throw (CustomException("Invalid port number [1023 <=> 65535]"));
	}
}

/*
**	At this point, the input is Format OK and has meaning. We now stock 
**	this input in the corresponding variables.
*/

void	ClientInputHandler::FillInput(int argc, char **argv)
{
	teamName = argv[2];
	port = stoi(argv[4], NULL, 10);
	if (argc == 7)
		hostName = argv[6];
	else
		hostName = "localhost";
}
