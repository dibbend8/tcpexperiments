//
// Generated file, do not edit! Created by opp_msgtool 6.0 from sdnmsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "sdnmsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(Sdm)

Sdm::Sdm(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

Sdm::Sdm(const Sdm& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

Sdm::~Sdm()
{
    delete [] this->rates;
}

Sdm& Sdm::operator=(const Sdm& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void Sdm::copy(const Sdm& other)
{
    delete [] this->rates;
    this->rates = (other.rates_arraysize==0) ? nullptr : new double[other.rates_arraysize];
    rates_arraysize = other.rates_arraysize;
    for (size_t i = 0; i < rates_arraysize; i++) {
        this->rates[i] = other.rates[i];
    }
}

void Sdm::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    b->pack(rates_arraysize);
    doParsimArrayPacking(b,this->rates,rates_arraysize);
}

void Sdm::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    delete [] this->rates;
    b->unpack(rates_arraysize);
    if (rates_arraysize == 0) {
        this->rates = nullptr;
    } else {
        this->rates = new double[rates_arraysize];
        doParsimArrayUnpacking(b,this->rates,rates_arraysize);
    }
}

size_t Sdm::getRatesArraySize() const
{
    return rates_arraysize;
}

double Sdm::getRates(size_t k) const
{
    if (k >= rates_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)rates_arraysize, (unsigned long)k);
    return this->rates[k];
}

void Sdm::setRatesArraySize(size_t newSize)
{
    double *rates2 = (newSize==0) ? nullptr : new double[newSize];
    size_t minSize = rates_arraysize < newSize ? rates_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        rates2[i] = this->rates[i];
    for (size_t i = minSize; i < newSize; i++)
        rates2[i] = 0;
    delete [] this->rates;
    this->rates = rates2;
    rates_arraysize = newSize;
}

void Sdm::setRates(size_t k, double rates)
{
    if (k >= rates_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)rates_arraysize, (unsigned long)k);
    this->rates[k] = rates;
}

void Sdm::insertRates(size_t k, double rates)
{
    if (k > rates_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)rates_arraysize, (unsigned long)k);
    size_t newSize = rates_arraysize + 1;
    double *rates2 = new double[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        rates2[i] = this->rates[i];
    rates2[k] = rates;
    for (i = k + 1; i < newSize; i++)
        rates2[i] = this->rates[i-1];
    delete [] this->rates;
    this->rates = rates2;
    rates_arraysize = newSize;
}

void Sdm::appendRates(double rates)
{
    insertRates(rates_arraysize, rates);
}

void Sdm::eraseRates(size_t k)
{
    if (k >= rates_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)rates_arraysize, (unsigned long)k);
    size_t newSize = rates_arraysize - 1;
    double *rates2 = (newSize == 0) ? nullptr : new double[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        rates2[i] = this->rates[i];
    for (i = k; i < newSize; i++)
        rates2[i] = this->rates[i+1];
    delete [] this->rates;
    this->rates = rates2;
    rates_arraysize = newSize;
}

class SdmDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_rates,
    };
  public:
    SdmDescriptor();
    virtual ~SdmDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(SdmDescriptor)

SdmDescriptor::SdmDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(Sdm)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

SdmDescriptor::~SdmDescriptor()
{
    delete[] propertyNames;
}

bool SdmDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Sdm *>(obj)!=nullptr;
}

const char **SdmDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *SdmDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int SdmDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int SdmDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_rates
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *SdmDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "rates",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int SdmDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "rates") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *SdmDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "double",    // FIELD_rates
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **SdmDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SdmDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SdmDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        case FIELD_rates: return pp->getRatesArraySize();
        default: return 0;
    }
}

void SdmDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        case FIELD_rates: pp->setRatesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'Sdm'", field);
    }
}

const char *SdmDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SdmDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        case FIELD_rates: return double2string(pp->getRates(i));
        default: return "";
    }
}

void SdmDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        case FIELD_rates: pp->setRates(i,string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Sdm'", field);
    }
}

omnetpp::cValue SdmDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        case FIELD_rates: return pp->getRates(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'Sdm' as cValue -- field index out of range?", field);
    }
}

void SdmDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        case FIELD_rates: pp->setRates(i,value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Sdm'", field);
    }
}

const char *SdmDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr SdmDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void SdmDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    Sdm *pp = omnetpp::fromAnyPtr<Sdm>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'Sdm'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

