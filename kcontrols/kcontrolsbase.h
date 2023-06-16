#ifndef KCONTROLSBASE_H
#define KCONTROLSBASE_H



enum ENUMKCONTROLTYPE
{
    ENUMKCONTROLTYPE_KLINEEDIT=0,
    ENUMKCONTROLTYPE_KSPINBOX=1,

};

class KControlsBase
{
public:
    KControlsBase(ENUMKCONTROLTYPE t);
    ENUMKCONTROLTYPE type()
    {
        return m_type;
    }
private:
    ENUMKCONTROLTYPE m_type;
};

#endif // KCONTROLSBASE_H
