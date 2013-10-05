#ifndef NODE_H
#define NODE_H

#include <QString>

class Node
{
public:
    Node();

    //Metodos get
    QString getName ();
    int getLevel ();
    QString getValue ();
    bool isUsed ();

    //Metodos set
    void setName (QString name);
    void setLevel (int level);
    void setValue (QString value);
    void setUsed (bool used);

private:
    QString name;
    int level;
    QString value;
    bool used;
};

#endif // NODE_H
