#include "servicecalculater.h"


bool ServiceCalculater::sumGreaterThan100(QString &hata, const QVector<int> &values, int &result)
{
    bool flag = false;
    result = 0;
    for(int i = 0; i < values.size(); ++i){
        result += values.at(i);
    }
    if(result > 100){
        flag = true;
    }else {
        hata = "send much bigger values";
    }
    return flag;
}

bool ServiceCalculater::call(QString &_method_name, TemplateReadersWriters *readerWriter)
{
    bool flag = false;
    if(_method_name == QString("sumGreaterThan100")){
        QString hata;
        QVector<int> values;
        int sum;
        readerWriter->unMarshall(values);
        flag = this->sumGreaterThan100(hata, values, sum);
        readerWriter->marshall(flag, hata, sum);
    }
    return flag;
}
