
/*
 *
 *  conn.h
 *
 *  Common interfaces for rdma connections
 *
 */

#ifndef KNY_ENDPOINT_HPP
#define KNY_ENDPOINT_HPP

#include <memory> // For smart pointers

#include <infiniband/verbs.h> 

#include "error.hpp"

namespace kym {
namespace endpoint {

struct Options {
  struct ibv_qp_init_attr  qp_attr;

  const void *private_data;
	uint8_t private_data_len;
	uint8_t responder_resources;
	uint8_t initiator_depth;
	uint8_t flow_control;
	uint8_t retry_count;		/* ignored when accepting */
	uint8_t rnr_retry_count;
};


class Endpoint {
  public:
    friend class Listener;

    ibv_pd GetPd();

    Status PostSendRaw(struct ibv_send_wr *wr, struct ibv_send_wr **bad_wr);
    Status PostSend(uint64_t ctx, uint32_t lkey, void *addr, size_t size);
    Status PostRead(uint64_t ctx, uint32_t lkey, void *addr, size_t size);
    Status PostWrite(uint64_t ctx, uint32_t lkey, void *addr, size_t size);
    StatusOr<ibv_wc> PollSendCq();

    Status PostRecvRaw(struct ibv_recv_wr *wr, struct ibv_recv_wr **bad_wr);
    Status PostRecv(uint64_t ctx, uint32_t lkey, void *addr, size_t size);
    StatusOr<ibv_wc> PollRecvCq();

  private:
    Endpoint();
};

class Listener {
  public:
    StatusOr<std::unique_ptr<Endpoint>> Accept(Options opts);
  private:
    Listener();
};

StatusOr<std::unique_ptr<Endpoint>> Dial(std::string ip, int port, Options opts);

StatusOr<std::unique_ptr<Listener>> Listen(std::string ip, int port);
StatusOr<std::unique_ptr<Listener>> Listen(std::string ip, int port, Options opts);

}
}

#endif // KNY_ENDPOINT_HPP
