// $Id: acnetex.h,v 1.1 2011/09/02 13:58:15 nicklaus Exp $
// A few acnet errors in the form of a C++ exception object
#include <stdexcept>
#define MOOC 57
#define FTPFAC 15
namespace FFF {

  class AcnetError : public std::exception {
  protected: 
    int facility;
    int error;
    int number;
    char buff[40];

  public:
    AcnetError(const int fac, const int err){
      facility = fac;
      error = err;
      number = (error << 8) | facility;
      sprintf(buff,"Acnet Exception %d, %d\n",facility, error);
    }
    virtual const char* what() const throw()
    {
      return buff;
    }
    int getValue() const{
      return number;
    }
  };

  static const  AcnetError Ex_MTVAL(MOOC, -3);
  static const AcnetError Ex_UNSUPMT(MOOC, -16);
  static const AcnetError Ex_BADSET(MOOC, -23);
  static const AcnetError Ex_BADOFF(MOOC, -28);
  static const AcnetError Ex_BADLEN(MOOC, -29);
  static const AcnetError Ex_BADCHAN(MOOC, -33);
  static const AcnetError Ex_BADOFLEN(MOOC, -34);
  static const AcnetError Ex_WRONGTP(MOOC, -37);
  static const AcnetError Ex_WRBASCON(MOOC, -38);
  static const AcnetError Ex_NOACCESSOR(MOOC, -105);
  static const AcnetError Ex_DEVNOTDEFINED(MOOC, -110);
  static const AcnetError Ex_DEVFAILED(MOOC, -111);
  static const AcnetError Ex_DEVNOTFOUND(MOOC, 1);
  static const AcnetError Ex_DEVNOTCONNECTED(MOOC, 2);


  static const AcnetError Ex_FTPLIMIT(FTPFAC, -8);
  static const AcnetError Ex_FTPFENOR(FTPFAC, -31);

}
