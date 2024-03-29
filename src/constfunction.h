#ifndef CONSTFUNCTION_H
#define CONSTFUNCTION_H

#include <QStringList>


#ifndef utr
# define utr QString::fromUtf8
#endif

enum Columns{
    CName,
    CSex,
    CAge,
    CHeight,
    CWeight,
    CNationality,
    CBirthday,
    CDeath
};

const QStringList columnsName {
            "Name",
            "Sex",
            "Age",
            "Height",
            "Weight",
            "Nationality",
            "DateByrday",
            "DateDeath"
};

#endif // CONSTFUNCTION_H
