#pragma once
#include "Connection.h"


namespace NL
{
		template<typename T>
		class ServerInterface
		{
		public:
			ServerInterface(uint16_t port)
				: m_asioAccepter(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
			{

			}

			virtual ~ServerInterface()
			{
				Stop();
			}

			bool Start()
			{
				try
				{
					WaitForClientConnection();

					m_threadContext = std::thread([this]() {m_asioContext.run(); });
				}
				catch (std::exception& e)
				{
					//TODO: Replace this with spdlog::error
					std::cerr << "[SERVER] Exception" << e.what() << "\n";
					return false;
				}
				//TODO: Replace this with spdlog::info
				std::cout << "[SERVER] Started!\n";
				return true;
			}

			void Stop()
			{
				m_asioContext.stop();

				if (m_threadContext.joinable())
				{
					m_threadContext.join();
				}

				std::cout << "[SERVER] Stooped!\n";
			}

			void WaitForClientConnection()
			{
				m_asioAccepter.async_accept(
					[this](std::error_code ec, asio::ip::tcp::socket socket)
					{
						if (!ec)
						{
							std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

							std::shared_ptr<connection<T>> newconn =
								std::make_shared<connection<T>>(connection<T>::owner::server,
									m_asioContext, std::move(socket), m_qMessagesIn);

							if (OnClientConnect(newconn))
							{
								m_deqConnections.push_back(std::move(newconn));
								m_deqConnections.back()->ConnectToClient(this,nIDCounter++);
								std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved\n";
							}
							else
							{
								std::cout << "[----] Connection Denied\n";
							}
						}
						else
						{
							std::cout << "[SERVER] New Connection Error: " << ec.Message() << "\n";
						}
						//Prime the asio context with more work, again simply wait for another Connection
						WaitForClientConnection();
					});
			}

			void MessageClient(std::shared_ptr<connection<T>> client, const Message<T>& msg)
			{
				if (client && client->IsConnected())
				{
					client->Send(msg);
				}
				else
				{
					OnClientDisconnect(client);
					client.reset();
					m_deqConnections.erase(
						std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end()
					);
				}
			}

			void MessageAllClients(const Message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr)
			{
				bool bInvalidClientExists = false;

				for (auto& client : m_deqConnections)
				{
					if (client && client->IsConnected())
					{
						if (client != pIgnoreClient)
							client->Send(msg);
					}
					else
					{
						OnClientDisconnect(client);
						client.reset();
						bInvalidClientExists = true;
					}
				}
				// Remove dead clients, all in one go - this way, we dont invalidate the container as we iterated through it.
				if (bInvalidClientExists)
				{
					m_deqConnections.erase(
						std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end()
					);
				}
			}

			void Update(size_t nMaxMessages = -1, bool bWait = false)
			{
				if (bWait) m_qMessagesIn.wait();

				size_t nMessageCount = 0;
				while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
				{
					auto msg = m_qMessagesIn.pop_front();
					OnMessage(msg.remote, msg.msg);
					nMessageCount++;
				}
			}

			virtual void OnClientValidated(std::shared_ptr<connection<T>> client)
			{
			}

		protected:

			virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
			{
				return false;
			}

			virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
			{
			}

			virtual void OnMessage(std::shared_ptr<connection<T>> client, Message<T>& msg)
			{
			}

			//Thread Safe Queue for incoming mesage packets
			TSQueue<OwnedMessage < T>> m_qMessagesIn;

			//Container of active validated connections
			std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

			//Order of declaration is important- it is also the order of initialisation
			asio::io_context m_asioContext;
			std::thread m_threadContext;

			//These things need an asio context
			asio::ip::tcp::acceptor m_asioAccepter;

			//Clients will be identified in the "wider system" via an ID
			uint32_t nIDCounter = 10000;

        };
}

