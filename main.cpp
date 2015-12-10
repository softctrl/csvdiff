#include <QVariant>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>
#include <QtWidgets>

#include "stringdata.h"
#include "reader.h"
#include "writer.h"

/**
 * @brief toString
 * @param value
 * @return
 */
QString toString(QStringList value) {
    QString _value("NULL");
    if (value.size() > 0) {
        _value = value.at(0);
        for (int idx = 1; idx < value.size(); idx++) {
            _value = _value.append(", ").append(value.at(idx));
        }
    }
    return _value;
}

/**
 * @brief showMessage
 * @param tex
 */
void showMessage (const QString &tex) {
    QMessageBox Msgbox;
    Msgbox.setText(tex);
    Msgbox.exec();
}

/**
 * @brief createKey
 * @param value
 * @param idx1
 * @param idx2
 * @param idx3
 * @return
 */
QString createKey(QStringList value, int idx1, int idx2, int idx3) {
    return QString("(").append(value.at(idx1)).append(")-(")
            .append(value.at(idx2)).append(")-(")
            .append(value.at(idx3)).append(")");
}

/**
 * @brief createKey
 * @param value
 * @return
 */
uint createKey(QStringList value) {
    return qHash(toString(value));
}

/**
 * @brief createHasMap
 * @param list
 * @return
 */
QMap<QString, QStringList> createHasMap(QList<QStringList> list, int idx1, int idx2, int idx3) {

    QMap<QString, QStringList> map;
    long long int count = list.size();

    //**********************************************************************
    QProgressDialog progress("Reading file 1...", "Cancel", 0, count, 0);//*
    progress.setWindowModality(Qt::WindowModal);                         //*
    //**********************************************************************

    for (long long int i = 1; i < count; ++i ) {
//        qDebug(list.at(1).at(0).toLatin1());
        QString key = createKey(list.at(i), idx1, idx2, idx3);
        if (map.contains(key)) {
            qDebug(QString("["+QString::number(i)+"]-Registro Duplicado[" + key + "]").toLatin1());
        } else {
            map.insert(key, list.at(i));
//            qDebug(toString(list.at(i)).toLatin1());
        }

        //**********************
        progress.setValue(i);//*
        //**********************

    }
    return map;

}

/**
  i need column 8
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    const QString SEPARATOR("#");

    QApplication a(argc, argv);
//    a.arguments()

//    QFileDialog::getSaveFileName()
    QString document = QFileDialog::getOpenFileName(0, "Open *.CSV file", QString(), "*.csv");
    //QString file1 = QDir::currentPath() + "/File00.csv";
    //QString file2 = QDir::currentPath() + "/File01.csv";    document
    QMap<QString, QStringList> map1 = createHasMap(QtCSV::Reader::readToList(document, SEPARATOR), 0, 11, 14);
    showMessage("Total:" + QString::number(map1.count()));
    document = QFileDialog::getOpenFileName(0, "Open *.CSV file", QString(), "*.csv");
    QMap<QString, QStringList> map2 = createHasMap(QtCSV::Reader::readToList(document, SEPARATOR), 0, 10, 13);
    showMessage("Total:" + QString::number(map2.count()));
    long long int total = 0;

    //*****************************************************************************
    long long int count = 0;                                                    //*
    QProgressDialog progress("Reading file 1...", "Cancel", 0, map2.count(), 0);//*
    progress.setWindowModality(Qt::WindowModal);                                //*
    //*****************************************************************************

    QtCSV::StringData data;
    foreach(QString key, map2.keys()) {

        QStringList value2 = map2.value(key);
        QString status = "";
        if (map1.contains(key)) {
            QStringList value1 = map1.value(key);
            status = value1.at(8);
            QString log("[");
            log = log.append(createKey(value1, 0, 11, 14))
                    .append("]->[")
                    .append(createKey(value2, 0, 10, 13))
                    .append("] => ").append(status);
            qDebug(log.toLatin1());
            total++;            
        }
        value2.append(status);
        data.addRow(value2);

        //****************************
        progress.setValue(++count);//*
        //****************************

    }
    QString file = QDir::currentPath() + "/FINAL_FILE.csv";
    QtCSV::Writer::write(file, data, SEPARATOR);
    showMessage("Fim.\nTotal: " + QString::number(total));
    a.exit(0);
    return 0;

}
