#include <bits/stdc++.h>
using namespace std;
int ERROR=0;
int NO_OF_WORDS(string s)
{
    int i=0,j=1;
    while(i<s.length())
    {
        if(s.at(i)==' ')
            j++;
        i++;
    }
    return j;
}
string first_word(string s)
{
    int i=0;
    while(i<s.length())
    {
        if(s.at(i)==' ')
        break;
        i++;
    }
    if(i!=s.length())
        return s.substr(0,i);
    else
        return s;
}
string second_word(string s)
{
    int i=0,j;
    while(i < s.length())
    {
        if(s.at(i) == ' ')
            break;
        i++;
    }
    j=i;
    while(j < s.length())
    {
        if(s.at(j) == ' ')
            break;
        j++;
    }
    if(j!=s.length())
        return s.substr(i+1,j);
    else
        return s.substr(i+1);
}
string third_word(string s)
{
    int i=0,j,k=2;
    while(i < s.length())
    {
        if(s.at(i) == ' ')
        {
            k--;
            if(k<0)
                break;
        }
        i++;
    }
    j=i;
    while(j < s.length())
    {
        if(s.at(j) == ' ')
            break;
        j++;
    }
    if(j!=s.length())
        return s.substr(i+1,j);
    else
        return s.substr(i+1);
}
int convert_to_int(string s)
{
    int j=s.length()-1,num=0;
    while(j>=0)
    {
        if(s.at(j)<=57 && s.at(j)>=48)
            num=num*16+(s.at(j)-48);
        else
            num=num*16+(s.at(j)-55);
        j--;
    }
    return num;
}
void pass1()
{
    map<string,pair<string,int>>OT;
    map<string,int>LT;
    //Defining OPTAB 
    fstream INSfile;
    INSfile.open("Optab.txt");
    while(!INSfile.eof())
    {
        string ins,oc;
        int length;
        INSfile>>ins>>oc>>length;
        OT[ins]=make_pair(oc,length);
    }
    INSfile.close();
    //OPTAB Defined and can be used as OT["LDA"].first
    //for respective object code and OT["LDA"].second
    //for length of instruction
    
    
    //Pass 1 intializing to make label tab and  assing addresses in integer format
    fstream PRGfile;
    fstream INTER_file;
    fstream Label_file;
    PRGfile.open("Input.txt");
    INTER_file.open("Intermediatefile.txt");
    Label_file.open("Label.txt");
    string prog_name,start,start_addr,str="START",ext="EXTDEF",exr="EXTREF",sect="CSECT";
    string resb="RESB",resw="RESW",wrd="WORD";
    PRGfile>>prog_name>>start>>start_addr;
    int add=0;
    if(strcmp(start.c_str(),str.c_str())==0)
    {
        while(!PRGfile.eof())
        {
            string line,opcode,label,length_of_word;
            getline (PRGfile , line);
            int n=NO_OF_WORDS(line);
            if(line.at(0)=='.')
            {
                INTER_file<<line<<endl;
                continue;
            }
            if(strcmp(first_word(line).c_str(),ext.c_str())==0)
            {
                INTER_file<<line<<endl;
                continue;
            }
            if(strcmp(first_word(line).c_str(),exr.c_str())==0)
            {
                INTER_file<<line<<endl;
                continue;
            }
            if(strcmp(second_word(line).c_str(),sect.c_str())==0)
            {
                INTER_file<<line<<endl;
                add=0;
                continue;
            }
            if(n==2 || n==1)
            {
                opcode = first_word(line);
                if(OT.find(opcode)!=OT.end())
                {
                    INTER_file<<line<<" "<<add<<endl;
                    if(OT[opcode].second == 0 && opcode.at(0)!='+')
                        add += 3;
                    else if(OT[opcode].second == 0)
                        add += 4;
                    else
                        add += OT[opcode].second;
                }
                else
                {
                    cout<<"ERROR:Object code not found"<<endl;
                    ERROR = 1;
                    break;
                }
            }
            else if(n==3)
            {
                label = first_word(line);
                opcode = second_word(line);
                if(OT.find(label)==OT.end() && LT.find(label)==LT.end())
                {
                    Label_file<<label<<" "<<add<<endl;
                    LT[label]=add;
                }
                else if(OT.find(label)!=OT.end())
                {
                    cout<<"ERROR:Label having same name as Object code not found"<<endl;
                    ERROR = 1;
                    break;
                }
                else
                {
                    cout<<"ERROR:Duplicate Label "<<endl;
                    ERROR = 1;
                    break;
                }
                if(OT.find(opcode)!=OT.end())
                {
                    INTER_file<<line<<" "<<add<<endl;
                    if(OT[opcode].second == 0 && opcode.at(0)!='+')
                        add += 3;
                    else if(OT[opcode].second == 0)
                        add += 4;
                    else
                        add += OT[opcode].second;
                }
                else if(strcmp(opcode.c_str(),resb.c_str())==0)
                {
                    length_of_word = third_word(line);
                    add +=  convert_to_int(length_of_word);
                }
                else if(strcmp(opcode.c_str(),resw.c_str())==0)
                {
                    length_of_word = third_word(line);
                    add +=3 * convert_to_int(length_of_word);
                }
                else if(strcmp(opcode.c_str(),wrd.c_str())==0)
                {
                    length_of_word = third_word(line);
                    add +=3 * convert_to_int(length_of_word);
                }
                else
                {
                    cout<<"ERROR:Object code not found"<<endl;
                    ERROR = 1;
                    break;
                }
            }
        }
    }
    PRGfile.close();
    INTER_file.close();
    Label_file.close();
}
int main()
{
    pass1();
    if(ERROR==0)
        pass2();
    return 0;
}
