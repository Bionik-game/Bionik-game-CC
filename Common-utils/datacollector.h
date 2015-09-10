#ifndef DATACOLLECTOR
#define DATACOLLECTOR

#include <QObject>
#include <exception>

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
    friend class DataCollector<T, Args...>; //External class can access private methods of internal one
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

