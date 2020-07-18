#ifndef MODELVIEWDEMOWIDGET_H
#define MODELVIEWDEMOWIDGET_H

#include <QWidget>
#include <QAbstractTableModel>

class QTableView;
class QLineEdit;

class PersonsModel;

// ********************************************************************************
class ModelViewDemoWidget : public QWidget {
    Q_OBJECT

public:
    ModelViewDemoWidget( QWidget* parent = 0 );
    ~ModelViewDemoWidget();

private slots:
    void onAppend();
    void onChangeData();

private:
    QTableView* m_view;
    PersonsModel* m_model;

    QLineEdit* m_surnameAddEdit;
    QLineEdit* m_ageAddEdit;
    QLineEdit* m_sexAddEdit;
    QLineEdit* m_surnameChangeEdit;
    QLineEdit* m_ageChangeEdit;
    QLineEdit* m_sexChangeEdit;
    QLineEdit* m_rowEdit;
};

// ********************************************************************************
class PersonsModel : public QAbstractTableModel {
    Q_OBJECT
public:
    PersonsModel( QObject* parent = 0 );

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendPerson( const QString& surname, const QString& age, const QString& sex );


public slots:
    void removeSelected();
    void readPersons();
    void changeData(const QString& surname, const QString& age, const QString& sex, const QString& row);

private:
    enum Column {
        SURNAME = 0,
        AGE,
        SEX,
        SELECTION,
        LAST
    };

    typedef QHash< Column, QVariant > PersonData;
    typedef QList< PersonData > Persons;
    Persons m_persons;

};

#endif // MODELVIEWDEMOWIDGET_H
