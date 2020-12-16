#ifndef SQLITER_BASIC_SQLITE_H
#define SQLITER_BASIC_SQLITE_H

#include "sqliter/basic_sqlite_service.h"

#include <boost/asio.hpp>
#include <string>
#include <type_traits>
namespace sqliter {
namespace asio {

template <typename Service>
class basic_sqlite : public boost::asio::basic_io_object<Service>
{
public:
  explicit basic_sqlite(boost::asio::io_service &io_service)
    : boost::asio::basic_io_object<Service>(io_service)
  {}

  void open(const std::string &db_name)
  {
    this->get_service().open(this->get_implementation(), db_name);
  }

  template <
      typename Handler,
      typename std::enable_if<std::is_same<
          Handler,
          boost::asio::executor_binder<
              std::_Bind<boost::asio::detail::promise_handler<
                  void(boost::system::error_code), std::allocator<void>>(std::_Placeholder<1>)>,
              boost::asio::detail::promise_executor<
                  void, boost::asio::execution::detail::blocking::possibly_t<0>>>>::value>::type * =
          nullptr>
  void async_query(const std::string &sql, Handler handler)
  {
    this->get_service().async_query(this->get_implementation(), sql, handler);
  }

  template <
      typename Handler,
      typename std::enable_if<std::is_invocable<
          Handler, boost::system::error_code,
          query_result<int32_t, int32_t, std::string, float>>::value>::type * = nullptr

      ,
      typename std::enable_if<!std::is_same<
          Handler,
          boost::asio::executor_binder<
              std::_Bind<boost::asio::detail::promise_handler<
                  void(boost::system::error_code), std::allocator<void>>(std::_Placeholder<1>)>,
              boost::asio::detail::promise_executor<
                  void, boost::asio::execution::detail::blocking::possibly_t<0>>>>::value>::type * =
          nullptr>
  bool async_query(const std::string &sql, Handler handler)
  {
    this->get_service().async_query1(this->get_implementation(), sql, handler);
    return 0;
  }

  template <typename query_result_type>
  void query(const std::string &sql, boost::system::error_code &ec, query_result_type &result)
  {
    this->get_service().query(this->get_implementation(), sql, ec, result);
  }

  void query(const std::string &sql, boost::system::error_code &ec)
  {
    this->get_service().query(this->get_implementation(), sql, ec);
  }

  void close(boost::system::error_code &ec)
  {
    this->get_service().close(this->get_implementation(), ec);
  }
};

}  // namespace asio
}  // namespace sqliter
#endif