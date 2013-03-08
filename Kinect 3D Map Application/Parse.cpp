#include "Parse.h"


vector<string> split(const string strValue, char separator)
{
    vector<string> vecstrResult;
    int startpos=0;
    int endpos=0;

    endpos = strValue.find_first_of(separator, startpos);
    while (endpos != -1)
    {       
        vecstrResult.push_back(strValue.substr(startpos, endpos-startpos)); // add to vector
        startpos = endpos+1; //jump past sep
        endpos = strValue.find_first_of(separator, startpos); // find next
        if(endpos==-1)
        {
            //lastone, so no 2nd param required to go to end of string
            vecstrResult.push_back(strValue.substr(startpos));
        }
    }

    return vecstrResult;
}

Parse::Parse()
{
}

Parse::Parse(string filename)
{
	this->m_filename = filename;
}


Parse::~Parse(void)
{
	//for (int i = 0; i < this->m_points.size(); i++)
	//{
	//	delete m_points[i];
	//}
}

void Parse::Add_point(Point p)
{
	this->m_points.push_back(p);
}

Point Parse::Get_point(int i)
{
	return m_points[i];
}

vector<Point> Parse::Get_points()
{
	return this->m_points;
}

bool Parse::Read()
{
	ifstream file;
	char buffer[2048];
	file.open(this->m_filename);
	string line;
	string end_header = "end_header";
	bool valid_file = false;

	int count = 0;
	while(!file.eof())
	{
		
		file.getline(buffer, 2048);
		line = buffer;

		if(!line.compare(end_header))
		{
			valid_file = true;
			continue;
		}

		if(valid_file)
		{
			if(line.compare(""))
			{
				this->m_points.push_back(Create_point(line));
				count++;
			}
		}
	}

	file.close();

	return valid_file;
}

inline Point Parse::Create_point(string line)
{
	Point point;
	vector<string> values = split(line, ' ');

	point.X = atof(values[0].c_str());
	point.Y = atof(values[1].c_str());
	point.Z = atof(values[2].c_str());
	point.R = atof(values[3].c_str());
	point.G = atof(values[4].c_str());
	point.B = atof(values[5].c_str());

	return point;
}