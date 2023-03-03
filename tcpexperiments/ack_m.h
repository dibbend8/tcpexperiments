//
// Generated file, do not edit! Created by opp_msgtool 6.0 from ack.msg.
//

#ifndef __ACK_M_H
#define __ACK_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class Ack;
/**
 * Class generated from <tt>ack.msg:33</tt> by opp_msgtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * message Ack
 * {
 *     //    int someField;
 *     //    string anotherField;
 *     double price1;
 *     double price2;
 *     int id;
 * 
 * }
 * </pre>
 */
class Ack : public ::omnetpp::cMessage
{
  protected:
    double price1 = 0;
    double price2 = 0;
    int id = 0;

  private:
    void copy(const Ack& other);

  protected:
    bool operator==(const Ack&) = delete;

  public:
    Ack(const char *name=nullptr, short kind=0);
    Ack(const Ack& other);
    virtual ~Ack();
    Ack& operator=(const Ack& other);
    virtual Ack *dup() const override {return new Ack(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual double getPrice1() const;
    virtual void setPrice1(double price1);

    virtual double getPrice2() const;
    virtual void setPrice2(double price2);

    virtual int getId() const;
    virtual void setId(int id);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Ack& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Ack& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline Ack *fromAnyPtr(any_ptr ptr) { return check_and_cast<Ack*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __ACK_M_H

