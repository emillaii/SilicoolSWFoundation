#ifndef CZINIFILE_H
#define CZINIFILE_H

#include <vector>
#include <string>

class IniFile
{
public:
    struct T_LineConf
    {
        std::string Key;
        std::string Value;
        std::string Comment;
        std::string Content; // Not standard key value pair, it is the part except comment
        T_LineConf()
        {
            Key = Value = Comment = Content = "";
        }
        T_LineConf(const std::string &key, const std::string &value, const std::string &comment, const std::string &content)
        {
            Key = key;
            Value = value;
            Comment = comment;
            Content = content;
        }
    };
    struct T_Section
    {
        std::string SectionName;
        std::vector<T_LineConf> SectionLines;
        T_Section()
        {
            SectionName = "";
            SectionLines.clear();
        }
        T_Section(const std::string &sectionName)
        {
            SectionName = sectionName;
            SectionLines.clear();
        }
    };
public:
    IniFile();
    virtual ~IniFile();

    std::string GetFileName() const { return m_fileName; }
    void SetCommentSign(const std::vector<std::string> &vecCommentSign);
    void LoadFile(const std::string &fileName);
    void SaveFile();
    void SaveAsFile(const std::string &fileName);
    std::string ReadSection(const std::string &sectionName);
    void ReadSection(const std::string &sectionName, std::vector<T_LineConf> &vecSection);
    int ReadInteger(const std::string &sectionName, const std::string &key);
    std::string ReadString(const std::string &sectionName, const std::string &key);
    bool WriteInteger(const std::string &sectionName, const std::string &key, int value);
    bool WriteString(const std::string &sectionName, const std::string &key, const std::string &value);


protected:
    std::vector<T_Section> m_iniContent;
    std::vector<std::string> m_vecCommentSign;
    std::string m_fileName;
    bool m_modified;

    std::string& ltrim(std::string &s);
    std::string& rtrim(std::string &s);
    std::string& trim(std::string &s);
    std::vector<std::string> split(const std::string &str, const std::string &spliter, bool skipEmptyParts = true);
};

#endif // CZINIFILE_H
