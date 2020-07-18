#include "modelviewdemowidget.h"
#include<QFileDialog>
#include <QTableView>
#include <QHeaderView>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

// ********************************************************************************
ModelViewDemoWidget::ModelViewDemoWidget( QWidget* parent ) : QWidget( parent ) {

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout( mainLayout );

    m_view = new QTableView;
    m_view->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
    m_view->setModel( m_model = new PersonsModel );
    mainLayout->addWidget( m_view );

    QGridLayout* panelLayout = new QGridLayout;
    mainLayout->addLayout( panelLayout );

    QPushButton* lbRemove = new QPushButton( trUtf8( "Удалить" ) );
    connect( lbRemove, SIGNAL( clicked() ),m_model, SLOT(removeSelected() ) );
    panelLayout->addWidget( lbRemove, 0, 6 );

    QPushButton* changeData = new QPushButton( trUtf8( "Изменить данные" ) );
    connect( changeData, SIGNAL( clicked() ), SLOT(onChangeData() ) );
    panelLayout->addWidget( changeData, 2, 6 );

    QLabel* lb1Surname = new QLabel( trUtf8( "Изменить: Фамилия" ) );
    panelLayout->addWidget( lb1Surname, 2, 0 );
    m_surnameChangeEdit = new QLineEdit;
    panelLayout->addWidget( m_surnameChangeEdit, 2, 1 );

    QLabel* lb1Name = new QLabel( trUtf8( "Возраст" ) );
    panelLayout->addWidget( lb1Name, 2, 2 );
    m_ageChangeEdit = new QLineEdit;
    panelLayout->addWidget( m_ageChangeEdit, 2, 3 );

    QLabel* lb1Patronymic = new QLabel( trUtf8( "Пол" ) );
    panelLayout->addWidget( lb1Patronymic, 2, 4 );
    m_sexChangeEdit = new QLineEdit;
    panelLayout->addWidget( m_sexChangeEdit, 2, 5 );

    QLabel* row = new QLabel( trUtf8( "Номер строки для изменения" ) );
    panelLayout->addWidget( row, 0, 0 );
    m_rowEdit = new QLineEdit;
    panelLayout->addWidget( m_rowEdit, 0, 1 );

    QPushButton* btnData = new QPushButton( trUtf8( "Открыть файл" ) );
    connect( btnData , SIGNAL( clicked() ),m_model, SLOT(readPersons() ) );
    panelLayout->addWidget( btnData, 0, 5 );

    QLabel* lbSurname = new QLabel( trUtf8( "Внести: Фамилия" ) );
    panelLayout->addWidget( lbSurname, 1, 0 );
    m_surnameAddEdit = new QLineEdit;
    panelLayout->addWidget( m_surnameAddEdit, 1, 1 );

    QLabel* lbName = new QLabel( trUtf8( "Возраст" ) );
    panelLayout->addWidget( lbName, 1, 2 );
    m_ageAddEdit = new QLineEdit;
    panelLayout->addWidget( m_ageAddEdit, 1, 3 );

    QLabel* lbPatronymic = new QLabel( trUtf8( "Пол" ) );
    panelLayout->addWidget( lbPatronymic, 1, 4 );
    m_sexAddEdit = new QLineEdit;
    panelLayout->addWidget( m_sexAddEdit, 1, 5 );

    QPushButton* bnAdd = new QPushButton( trUtf8( "Добавить" ) );
    connect( bnAdd, SIGNAL( clicked() ), SLOT( onAppend() ) );
    panelLayout->addWidget( bnAdd, 1, 6 );

    resize( 800, 600 );
}

ModelViewDemoWidget::~ModelViewDemoWidget() {
}

void ModelViewDemoWidget::onAppend() {
    m_model->appendPerson(
        m_surnameAddEdit->text(),
        m_ageAddEdit->text(),
        m_sexAddEdit->text()
    );
}

void ModelViewDemoWidget::onChangeData(){
    m_model->changeData(
        m_surnameChangeEdit->text(),
        m_ageChangeEdit->text(),
        m_sexChangeEdit->text(),
        m_rowEdit->text()

    );
}

// ********************************************************************************
PersonsModel::PersonsModel( QObject* parent ) : QAbstractTableModel( parent ) {
}

int PersonsModel::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return m_persons.count();
}

int PersonsModel::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return LAST;
}

QVariant PersonsModel::data( const QModelIndex& index, int role ) const {
    if(
        !index.isValid() ||
        m_persons.count() <= index.row() ||
        ( role != Qt::DisplayRole && role != Qt::EditRole )
    ) {
        return QVariant();
    }

    return m_persons[ index.row() ][ Column( index.column() ) ];
}

bool PersonsModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || m_persons.count() <= index.row() ) {
        return false;
    }

    m_persons[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}

QVariant PersonsModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case SURNAME:
        return trUtf8( "Фамилия" );
    case AGE:
        return trUtf8( "Возраст" );
    case SEX:
        return trUtf8( "Пол" );
    case SELECTION:
        return trUtf8( "Выбор" );
    }

    return QVariant();
}

Qt::ItemFlags PersonsModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() ) {
        if( index.column() == SELECTION ) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

void PersonsModel::appendPerson( const QString& surname, const QString& age, const QString& sex ) {
    PersonData person;
    person[ SURNAME ] = surname;
    person[ AGE ] = age;
    if(sex == "1") person[ SEX ] = "Male";
        else if (sex == "0") {
            person[ SEX ] = "Female";
}
    person[ SELECTION ] = false;

    int row = m_persons.count();
    beginInsertRows( QModelIndex(), row, row );
    m_persons.append( person );
    endInsertRows();
}

void PersonsModel::removeSelected() {
    int i = 0;
    Persons::iterator it = m_persons.begin();
    while( it != m_persons.end() ) {
        if( it->value( SELECTION, false ).toBool() ) {
            beginRemoveRows( QModelIndex(), i, i );
            it = m_persons.erase( it );
            endRemoveRows();
        } else {
            ++i;
            ++it;
        }

        if( it == m_persons.end() ) {
            break;
        }
    }
}
void PersonsModel::readPersons(){


    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.txt");
    QFile temp(str);
    QString surname;
    QString age;
    QString sex;
          if (temp.open(QIODevice::ReadOnly)) {
              while (!temp.atEnd()) {
                QString line = temp.readLine();
                QStringList list = line.split(" ");
                surname = (((list[0]).replace(" ","")).replace("\r","")).replace("\n","");
                age = (((list[1]).replace(" ","")).replace("\r","")).replace("\n","");
                sex = (((list[2]).replace(" ","")).replace("\r","")).replace("\n","");
                appendPerson(surname,age,sex);
              }
           //out << Qt::endl;
           }

}
void PersonsModel::changeData(const QString& surname, const QString& age, const QString& sex, const QString& row){
    //QAbstractItemModel::index(row.toInt(), 0);
    //void *ptr = nullptr;
    //[protected] QModelIndex QAbstractItemModel::createIndex(row.toInt(), 0) const;
    auto new_index=createIndex(row.toInt(), 0);
    auto new_index1=createIndex(row.toInt(), 1);
    auto new_index2=createIndex(row.toInt(), 2);
    //QModelIndex index = (row.toInt(),0);
    m_persons[ row.toInt() ][ Column( 0 ) ] = surname;
    m_persons[ row.toInt() ][ Column( 1 ) ] = age;
    if(sex == "1") m_persons[ row.toInt() ][ Column( 2 ) ] = "Male";
        else if (sex == "0") {
            m_persons[ row.toInt() ][ Column( 2 ) ] = "Female";}
    m_persons[ row.toInt() ][ Column( 3 ) ] = false;
    emit dataChanged( new_index, new_index );
    emit dataChanged( new_index1, new_index1 );
    emit dataChanged( new_index2, new_index2 );
}
