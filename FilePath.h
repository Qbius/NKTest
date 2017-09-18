#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

#include <string>
#include <vector>
#include <QtWidgets>
#include <fstream>

namespace file
{
    class path
    {
        QString fullpath;

    public:

        path(const QString& p) : fullpath(p.contains(".") ? absolute_file_name(QFileInfo{ p }) : QDir{ p }.absolutePath())
        {
        }

        path(const QString&& p) : path(p)
        {
        }

        path(const std::string& p) : path(QString::fromStdString(p))
        {
        }

        path(const path& p) : fullpath(p.fullpath)
        {
        }

        path()
        {
        }

        path operator/(const QString& rhs) const
        {
            return fullpath + "/" + rhs;
        }

        path operator+(const QString& rhs) const
        {
            return *this / rhs;
        }

        operator QString() const
        {
            return fullpath;
        }

        void create() const
        {
            if (!fullpath.contains("."))
                QDir{}.mkpath(fullpath);
        }

        std::vector<QString> read() const
        {
            std::vector<QString> lines;

            std::ifstream file(fullpath.toStdString());
            std::string line;
            while (std::getline(file, line))
            {
                lines.push_back(QString::fromStdString(line));
            }

            //QFile file{ fullpath };
            //if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
            //{
            //    QTextStream ts(&file);
            //    while (!ts.atEnd())
            //    {
            //        lines.push_back(ts.readLine());
            //    }
            //}

            return lines;
        }

        void write(const std::vector<QString>& lines)
        {
            QFile file{ fullpath };
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream ts(&file);
                for (const auto& line : lines)
                {
                    ts << line << '\n';
                }
            }
        }


        std::vector<QString> read_inside(const QString& filename) const
        {
            return path{ *this / filename }.read();
        }

        void write_inside(const QString& filename, const std::vector<QString>& lines) const
        {
            path{ *this / filename }.write(lines);
        }

        QString basename() const
        {
            QFileInfo file{ fullpath };
            return file.exists() ? file.baseName() : "";
        }

        QString extension() const
        {
            QFileInfo file{ fullpath };
            return file.exists() ? file.suffix() : "";
        }

        void rename(const QString& new_name)
        {
            QFileInfo file{ fullpath };
            if (file.exists())
            {
                QString old_filename = fullpath;
                fullpath.replace(file.baseName() + ".", new_name + ".");
                QFile::rename(old_filename, fullpath);
            }
        }
        
        void change_extension(const QString& new_ext)
        {
            QFileInfo file{ fullpath };
            if (file.exists())
            {
                QString old_filename = fullpath;
                fullpath.replace("." + file.suffix(), "." + new_ext);
                QFile::rename(old_filename, fullpath);
            }
        }

        void rename_inside(const QString& filename, const QString& new_name) const
        {
            path{ *this / filename }.rename(new_name);
        }

        std::vector<path> get_files() const
        {
            std::vector<path> files;

            QDir dir{ fullpath };
            if (dir.exists())
            {
                dir.setFilter(QDir::Files);
                for (const auto& file : dir.entryInfoList())
                {
                    files.push_back(absolute_file_name(file));
                }
            }

            return files;
        }

        path copy_to(const path& destination) const
        {
            QString full_destination = destination.fullpath + "/" + basename() + "." + extension();
            QFile::copy(fullpath, full_destination);
            return full_destination;
        }

        path parent() const
        {
            return QFileInfo{ fullpath }.absolutePath();
        }

        QString absolute_file_name(const QFileInfo& file) const
        {
            return file.absolutePath() + "/" + file.baseName() + ((file.suffix() != "") ? "." + file.suffix() : "");
        }

    };
}

#endif