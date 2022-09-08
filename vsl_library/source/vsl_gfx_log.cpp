////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_log.cpp ----------
/*!
\file vsl_gfx_log.cpp
\brief Implementation of the Gfx_Log class
\author Gareth Edwards

\note lots of stuff hardwired...

\note

	Pimpl_Gfx_Log methods:
		HRESULT CleanupLogFiles();
		HRESULT CreateLogFile();

	Really need to be checked for errors and faulty logic!

*/


#include "../header/vsl_gfx_log.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;

/*

notes


*/

// ---------- Private Implementation of Gfx_Log::Pimpl_Gfx_Log class ----------
class Gfx_Log::Pimpl_Gfx_Log
{

	public:

	// ---- useful
		enum
		{
			NORMAL,
			BANNER
		};

	// ---- cdtor
		Pimpl_Gfx_Log()
		{
			CreateLogFile();
		}
		~Pimpl_Gfx_Log() { ; }

	// ---- methods
		std::string GetLogDateTime();

		HRESULT CleanupLogFiles();
		HRESULT CreateLogFile();
		HRESULT Write(BOOL banner, std::string line);

	// ---- properties

	// ---- state
		BOOL show_date = FALSE;       // TRUE  - date (e.g., 22:03:11) 
		BOOL show_time = TRUE;        // TRUE  - time (e.g., 13:49:05:105)
		BOOL show_line_number = TRUE; // TRUE  - line (e.g., 102 @ )
		BOOL show_simple = TRUE;      // FALSE - no line number, date time stamp

	// ---- log folder names
		CHAR *log_folder = "Documents";
		CHAR *log_sub_folder = "Logs";

	// ---- log folders
		CHAR pathname[1024];          // log folder (e.g., "C:Users/[user]/Documents/Log/")
		CHAR filepathname[1024];      // log folder (e.g., "C:Users/[user]/Documents/Log/20220311_155540_log_file.txt")

	// ---- counter
		UINT log_file_max = 5;        // fewer the better... ;-)
		UINT line_number = 1;         // rolls over at 10000 to 1

	private:

};


////////////////////////////////////////////////////////////////////////////////


// ---------- GetLogTime ----------
/*!
\brief Get log date time stamp
\author Gareth Edwards
\param std::string - returned time stamp
\return 
*/
std::string Gfx_Log::Pimpl_Gfx_Log::GetLogDateTime()
{

	// ---- lambda function to cleanup date & time
		auto cleanup = [](CHAR *line)
		{
			INT len_line = (INT)strlen(line);
			for (INT i = 0; i < len_line - 3; i++)
				line[i] = line[i] == ' ' ? '0' : line[i];
		};

	// ---- use CHAR to create number, date and time char strings

	// --- line number
		CHAR number[16] = "";
		if (show_line_number)
		{
			// ---- format not to exceed "9999"!
				CHAR format[16] = "";
				if (line_number < 10)
					sprintf_s(format, 16, "%1d", line_number);
				else if ( line_number < 100)
					sprintf_s(format, 16, "%2d", line_number);
				else if (line_number < 1000)
					sprintf_s(format, 16, "%3d", line_number);
				else
					sprintf_s(format, 16, "%4d", line_number);

			// ---- copy with format
				sprintf_s(
						number,
						16,
						format,
						line_number
					);
		}

	// ---- get time NOW (GetSystemTime() returns the current time in UTC)
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

	// ---- date
		CHAR date[16] = "";
		if (show_date)
		{
			UINT year = (UINT)(SystemTime.wYear) - 2000;
			sprintf_s(
					date,
					16,
					"%2d/%2d/%2d",
					year,
					SystemTime.wMonth,
					SystemTime.wDay
				);
			cleanup(date);
		}

	// ---- time
		CHAR time[16] = "";
		if (show_time)
		{
			sprintf_s(
					time,
					16,
					"%2d:%2d:%2d:%3d",
					SystemTime.wHour,
					SystemTime.wMinute,
					SystemTime.wSecond,
					SystemTime.wMilliseconds
				);
			cleanup(time);
		}

	// ---- aggregate
		CHAR line[96] = "";
		strcat_s(line, 96, number);
		if (strlen(number) > 0) strcat_s(line, 96, " @ ");
		strcat_s(line, 96, date);
		if (strlen(date) > 0) strcat_s(line, 96, "-");
		strcat_s(line, 96, time);
		if (strlen(time) > 0) strcat_s(line, 96, ":");

	// ---- assign, allocating mem within scope of invoke method 
		std::string time_stamp = line;

	return time_stamp;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- CleanupLogFiles ----------
/*!
\brief Cleanup log files
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Log::Pimpl_Gfx_Log::CleanupLogFiles()
{

	// ---- local
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA fdata;
		std::stack<std::string> folders;
		std::string full_path;

	// ---- counter
		INT file_counter = 0;

	// ---- two passes
		for (UINT pass = 0; pass<2; pass++)
		{

			// ---- current file counter
				INT current_file_counter = 0;

			// ---- store
				std::string current_path = pathname;
				folders.push(current_path.c_str());

			// ---- recurse until no more folders
				while (!folders.empty())
				{

					// ---- init current path from folders stack top
						current_path = folders.top();

					// ---- init full directory path..
						full_path = current_path + "\\" + "*";

					// ---- then pop folders stack
						folders.pop();

					// FOR -- all files in this folder
						hFind = FindFirstFile(full_path.c_str(), &fdata);
						if ( hFind != INVALID_HANDLE_VALUE )
						{

							do
							{

								if (strcmp(fdata.cFileName, ".") != 0 &&
									strcmp(fdata.cFileName, "..") != 0)
								{

									// ---- folder ? - push current path onto foldersa stack
										if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
										{
											folders.push(current_path + "\\" + fdata.cFileName);
										}
									// ---- files ? - which are listed AFTER folders
										else
										{
											// ---- increment # file counter
												current_file_counter++;

											// ---- first pass ? - count # of files 
												if (pass == 0)
												{
													file_counter++;
												}
											// ---- delete excess
												else
												{
													// ---- file # less than # to be retained -- THEN -- delete
														if (current_file_counter <= file_counter - (INT)log_file_max)
														{
															CHAR paf[1024];
															sprintf_s(
																	paf,
																	1024,
																	"%s%s",
																	current_path.c_str(),
																	fdata.cFileName
																);
															BOOL result = DeleteFile(paf);
														}
												}
										}
								}
							} while (FindNextFile(hFind, &fdata) != 0);
						}

						// ---- check that this folder listing is complete ----
							if (GetLastError() != ERROR_NO_MORE_FILES)
							{
								FindClose(hFind);
								return ERROR_FAIL;
							}

						// ---- close this folder ----
							FindClose(hFind);
							hFind = INVALID_HANDLE_VALUE;

				}

		}

	return SUCCESS_OK;
}


// ---------- CreateLogFile ----------
/*!
\brief create log file
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)

\note 
	
	GetLocalTime - returns the system time adjusted for your current timezone
	GetSystemTime - returns Coordinated Universal Time (UTC)

	// this takes the time of your computer
		CTime t = CTime::GetCurrentTime();

	//this formats how do you want to appear in the string, e.g. 10:30:05
		strTime = t.Format("{%H:%M:%S} ");
		MessageBox(strTime, MB_OK);

*/
HRESULT Gfx_Log::Pimpl_Gfx_Log::CreateLogFile()
{

	// ---- local
		CHAR path[1024];

	// ---- get user directory path
		CHAR *userProfile;
		size_t userProfileLen;
		_dupenv_s(&userProfile, &userProfileLen, "USERPROFILE");
		strcpy_s(path, 1024, userProfile);
		size_t len = strlen(path);
		for ( UINT i=0; i<(UINT)len; i++ ) path[i] = path[i] == '\\' ? '/' : path[i];

	// ---- append Appdata/local
		strcat_s(path, 1024, "/");


	// ---- exists ?
		if ( GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES )
		{

			// ---- GetLocalTime - returns the system time adjusted for your current timezone
				//  GetSystemTime - returns Coordinated Universal Time (UTC)
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime);

			// ---- e.g., "Documents" folder does not exist -- THEN -- create 
				strcat_s(path, 1024, log_folder);
				strcat_s(path, 1024, "/");
				if ( GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES) INT result = _mkdir(path);

			// ---- e.g., "Logs" folder does not exist -- THEN -- create 
				strcat_s(path, 1024, log_sub_folder);
				strcat_s(path, 1024, "/");
				if ( GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES ) INT result = _mkdir(path);

			// ---- store
				strcpy_s(pathname, 1024, path);

			// ---- delete excess log files @ pathname
				HRESULT hr = CleanupLogFiles();
	
			// ---- catenate log date time to create filename
				char dt[256];
				sprintf_s(dt, 256, "%4d%2d%2d_%2d%2d%2d_log_file.txt",
					SystemTime.wYear,
					SystemTime.wMonth,
					SystemTime.wDay,
					SystemTime.wHour,
					SystemTime.wMinute,
					SystemTime.wSecond);
				size_t len = strlen(dt);
				for ( UINT i=0; i<(UINT)len; i++ ) dt[i] = dt[i] == ' ' ? '0' : dt[i];
				strcat_s(path, 1024, dt);

			// ---- store
				strcpy_s(filepathname, 1024, path);

			// ---- create & wipe
				std::ofstream log_file(path, std::ios_base::out);

			// ---- title
				std::ofstream app_log_file(path, std::ios_base::out | std::ios_base::app);
				app_log_file << "VSL Log File" << std::endl;

			// ---- date
				char date[256];
				sprintf_s(date, 256, "%4d/%2d/%2d",
					SystemTime.wYear,
					SystemTime.wMonth,
					SystemTime.wDay
				);
				len = strlen(date);
				for (UINT i = 0; i < (UINT)len; i++) date[i] = date[i] == ' ' ? '0' : date[i];

			// ---- time
				char time[256];
				sprintf_s(time, 256, "%2d:%2d:%2d",
						SystemTime.wHour,
						SystemTime.wMinute,
						SystemTime.wSecond
					);
				len = strlen(time);
				for (UINT i = 0; i < (UINT)len; i++) time[i] = time[i] == ' ' ? '0' : time[i];

			// ---- ms
				char ms[256];
				sprintf_s(ms, 256, "%3d",
						SystemTime.wMilliseconds
					);
				len = strlen(ms);
				for (UINT i = 0; i < (UINT)len; i++) ms[i] = ms[i] == ' ' ? '0' : ms[i];

			// ---- log
				app_log_file << "Created on (YMD) ";
				app_log_file << date;
				app_log_file << " at (HMS) ";
				app_log_file << time;
				app_log_file << " and (MS) ";
				app_log_file << ms << std::endl;

		}

		// ELSE -- report via message box AND then exit
		else
		{
			INT result = ::MessageBoxA(NULL,
					"Could not create/find the specified logs folder!\n\nPlease correct, then restart application.",
					"VSL Error",
					MB_OK | MB_ICONEXCLAMATION
				);
			exit(0);
		}

	return  SUCCESS_OK;

}


////////////////////////////////////////////////////////////////////////////////


// ---------- Write ----------
/*!
\brief write number, date, time and message
\author Gareth Edwards
\param BOOL - banner, if TRUE then don't prefix with number, date & time
\param std::string - message
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Log::Pimpl_Gfx_Log::Write(BOOL banner, std::string line)
{

	// ---- get timestamp, then either zap (if banner) or use
		std::string date_time = GetLogDateTime();
		if (banner)
		{
			// ----
				CHAR replacent_char = ' ';
				size_t dt_len = date_time.length();

			// ---- Using constructor:
				//	std::string empty(dt_len, replacent_char);
				//	date_time = empty;

			// ---- Using replace method:
				//	date_time.replace(0, dt_len, dt_len, replacent_char);

			// ---- Using std::fill algorithm (no particular reason for my choice, just fun):
				std::fill(date_time.begin() + 0, date_time.begin() + dt_len, replacent_char);
		}
		else
		{
			// ---- increment line number
				line_number = line_number >= 9999 ? 1 : line_number + 1;
		}

	// ---- log file
		std::ofstream log_file(filepathname, std::ios_base::out | std::ios_base::app);
		if (!show_simple)
			log_file << date_time.c_str() << "  " << line.c_str() << std::endl;
		else
			log_file << line.c_str() << std::endl;

	// ---- console
		if (!show_simple)
		{
			OutputDebugString(date_time.c_str());
			OutputDebugString(" ");
		}
		OutputDebugString(line.c_str());
		OutputDebugString("\n");

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- ctor ----------

Gfx_Log::Gfx_Log() : pimpl_gfx_log(new Pimpl_Gfx_Log)
{
	;
}

Gfx_Log::~Gfx_Log()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Set ----------
/*!
\brief set state parameters
*/
VOID Gfx_Log::SetShowDate(BOOL show_date)
	{ pimpl_gfx_log->show_date = show_date; }

VOID Gfx_Log::SetShowTime(BOOL show_time)
	{ pimpl_gfx_log->show_time = show_time; }

VOID Gfx_Log::SetShowLineNumber(BOOL show_line_number)
	{ pimpl_gfx_log->show_line_number = show_line_number; }

VOID Gfx_Log::SetShowSimple(BOOL show_simple)
	{ pimpl_gfx_log->show_simple = show_simple; }


// ---- note to self: 
//	std::string s = "+++ Timed Event" + std::to_string(seconds_elapsed) + "\n";


// ---------- Write ----------
/*!
\brief write ONLY datetime
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Log::Write(VOID)
{
	std::string line = "";
	pimpl_gfx_log->Write(pimpl_gfx_log->NORMAL, line);
	return SUCCESS_OK;
}

// ---------- Write ----------
/*!
\brief write datetime + CHAR message
\author Gareth Edwards
\param CHAR *message
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Log::Write(CHAR *message)
{
	std::string line = message;
	pimpl_gfx_log->Write(pimpl_gfx_log->NORMAL, line);
	return SUCCESS_OK;
}

// ---------- Write ----------
/*!
\brief write datetime + std::string message
\author Gareth Edwards
\param std::string message
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Log::Write(std::string message)
{
	pimpl_gfx_log->Write(pimpl_gfx_log->NORMAL, message);
	return SUCCESS_OK;
}

// ---------- WriteBanner ----------
/*!
\brief write 3 line banner with centred std::string message
\author Gareth Edwards
\param std::string banner_message
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Log::WriteBanner(std::string banner_message)
{
	WriteBannerLine();
	WriteBannerMessage(banner_message);
	WriteBannerLine();
	return SUCCESS_OK;
}

// ---------- WriteBannerLine ----------
/*!
\brief write single line banner
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Log::WriteBannerLine()
{
	// ---- double four times to 80 char
	std::string line = "*****";
	for (UINT i = 0; i < 4; i++) line += line;
	pimpl_gfx_log->Write(pimpl_gfx_log->BANNER, line);
	return SUCCESS_OK;
}

// ---------- WriteBanner ----------
/*!
\brief write single line banner with centred std::string message
\author Gareth Edwards
\param std::string banner_message
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Log::WriteBannerMessage(std::string banner_message)
{
	// ---- start with 5 char 
		std::string line = "     ";

	// ---- double four times to build empty 80 char line 
		for (UINT i = 0; i < 4; i++) line += line;

	// ---- copy centred banner message into line
		size_t bml = banner_message.length();
		UINT bm_at = 0;
		UINT at_start = 40 - (UINT)bml / 2;
		for (UINT at = at_start; at < at_start + (UINT)bml; at++)
			line.at(at) = banner_message.at(bm_at++);

	// ---- top & tail
		line.at(0) = '*';
		line.at(79) = '*';

	// ---- log
		pimpl_gfx_log->Write(pimpl_gfx_log->BANNER, line);

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////