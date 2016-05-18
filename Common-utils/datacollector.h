#ifndef DATACOLLECTOR
#define DATACOLLECTOR

#include <QObject>
#include <exception>

/**
 * W tym pliku najważniejsza jest klasa DataCollector i jej publiczne metody "set" oraz "get".
 * Idea tej klasy to synchronizacja sygnałów wiadomości. W momencie przyjścia pewnego podzbioru sygnałów,
 * nalezy wywołać metodę "set". Kiedy wszystkie zadeklarowane w argumentach szablonu obiekty przyjdą,
 * zostanie wysłany sygnał "allDataNew". W obsłudze tego sygnału można pobrać wiadomości z sygnałów
 * metodą "get". Numery w nawiasie trójkątnym odpowiadają kolejności podania argumentów szablonu.
 *
 * Przykład:
 *
 * class ExampleDataCollector
 * {
 * private:
 *      DataCollector<double, int, QString> dataCollector;
 *
 * public:
 *      ExampleDataCollector()
 *      {
 *          QObject::connect(&dataCollector, &DataCollector::allDataNew(), this, &ExampleData::processData);
 *      }
 *
 * public slots:
 *      void doubleArrived(double data)
 *      {
 *          dataCollector.set(0, data);
 *      }
 *      void intArrived(int data)
 *      {
 *          dataCollector.set(1, data);
 *      }
 *      void qStringArrived(QString data)
 *      {
 *          dataCollector.set(2, data);
 *      }
 *
 * private slots:
 *      void processData()
 *      {
 *          double dataOne = dataCollector.get<double>(0);
 *          int dataTwo = dataCollector.get<int>(1);
 *          QString dataThree = dataCollector.get<QString>(2);
 *
 *          // Do something with data
 *      }
 * }
 *
 *
 * QObject::connect(&dataCollector, &DataCollector::allDataNew, [](){
 *
 * })
 */

class SignalClass : public QObject
{
    Q_OBJECT
signals:
    void allDataNew();
    void dataDropped(unsigned int no);
};

class LastTag
{
public:
    template<typename T>
    LastTag& operator=(T&)
    {
        throw std::runtime_error("Something is implemented wrong! LastTag& LastTag::operator=(T&)");
    }
};

template<typename T = LastTag, typename...Args>
class DataCollector : public SignalClass
{
    template<typename X>
    friend class DataCollector<T, Args...>; //Without this you cannot access private methods of attribute "dataCollector"
private:
    template<typename S>
    class DataWrapper
    {
    private:
        S* data;
    public:
        class DataNotSetExceptionDataWrapper : std::exception {
        public:
            const char *what() const noexcept {return "Data for this element has not been yet set";}
        };

        DataWrapper()
            : data(nullptr)
        {
        }

        ~DataWrapper()
        {
            if(data != nullptr)
                delete data;
        }

        DataWrapper& operator=(const S& operand)
        {
            if(data != nullptr)
                delete data;
            data = new S(operand);
        }
        template<typename K>
        DataWrapper& operator=(const K&)
        {
            throw std::runtime_error("Something is implemented wrong! DataWrapper& DataWrapper::operator=(K& operand)");
        }

        void getData(S*& t) throw(DataNotSetExceptionDataWrapper)
        {
            if (data == nullptr)
                throw DataNotSetExceptionDataWrapper();
            t = data;
        }
        template<typename K>
        void getData(K*&)
        {
            throw std::runtime_error("Something is implemented wrong! void DataWrapper::getData(K& k)");
        }
    };

    DataWrapper<T> element;
    bool changed;
    DataCollector<Args...>* dataCollector;

    template<typename K>
    void setRecursive(unsigned int no, const K& element)
    {
        if (typeid(T) == typeid(LastTag))
            throw std::out_of_range("no value has exceeded the size of this DataCollector");
        else if(no == 0)
        {
            this->element = element;
            if (changed == true) //data was set twice or more before it was get
                emit dataDropped(no);
            else
                changed = true;
        }
        else
            dataCollector->setRecursive(no-1, element);
    }

    template<typename K>
    K* getRecursive(unsigned int no)
    {
        if (typeid(T) == typeid(LastTag))
            throw std::out_of_range("no value has exceeded the size of this DataCollector");
        else if(no == 0)
        {

            try
            {
                changed = false;
                K* data;
                element.getData(data);
                return data;
            }
            catch (typename DataWrapper<T>::DataNotSetExceptionDataWrapper&)
            {
                throw DataNotSetException();
            }
        }
        else
            return dataCollector->getRecursive<K>(no-1);
    }

    bool checkChangedRecursive()
    {
        if (typeid(T) == typeid(LastTag))
            return true;
        else
            return changed && dataCollector->checkChangedRecursive();
    }

public:
    class DataNotSetException : std::exception {
    public:
        const char *what() const noexcept {return "Data for this element has not been yet set";}
    };

    DataCollector()
        : changed(false)
    {
        if (typeid(T) == typeid(LastTag))
            dataCollector = nullptr;
        else
            dataCollector = new DataCollector<Args...>();
    }
    ~DataCollector()
    {
        if (dataCollector != nullptr)
            delete dataCollector;
    }

    template<typename K>
    void set(unsigned int no, const K& element)
    {
        setRecursive(no, element);

        if(checkChangedRecursive())
            emit allDataNew();
    }

    template<typename K>
    K get(unsigned int no)
    {
        return *(getRecursive<K>(no));
    }
};

#endif // DATACOLLECTOR

