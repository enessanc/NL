#pragma once
#include "Connection.h"


namespace NL
	{
		template <typename T>
		class ClientInterface
		{
		
		public:
			ClientInterface() : m_socket(m_context)
			{
				//Initialise the socket with the io context, so it can do stuff
			}


			virtual ~ClientInterface()
			{
				// If the client is destroyed, always try and disconnect from server
				Disconnect();
			}

			//Connect to server with hostname/ip-address and port
			bool Connect(const std::string& host, const uint16_t& port)
			{
				try
				{
					//Resolve hostname/ip-address inti tangiable physical address
					asio::ip::tcp::resolver resolver(m_context);
					asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

					//Create Connection
					m_connection = std::make_unique<Connection<T>>(
						Connection<T>::owner::client,
						m_context,
						asio::ip::tcp::socket(m_context),
						m_qMessagesIn
						);

					//Tell the Connection object to connect the server
					m_connection->ConnectToServer(endpoints);

					//Start Context Thread
					thrContext = std::thread([this]() {m_context.run(); });

				}
				catch (std::exception& e)
				{
					//TODO: Replace this line with appropriate logging module
					std::cerr << "FClient Exception: " << e.what() << "\n";
					return false;
				}

				return true;
			}
		
			void Disconnect()
			{
				if (IsConnected())
				{
					m_connection->Disconnect();
				}

				//Either way, we are also done with the asio context and its thread
				m_context.stop();
				if (thrContext.joinable())
				{
					thrContext.join();
				}

				m_connection.release();
			}
			
			bool IsConnected() const
			{
				if (m_connection)
				{
					return m_connection->IsConnected();
				}
				return false;
			}
			TSQueue<OwnedMessage<T>>& InComing()
			{
				return m_qMessagesIn;
			}

			void Send(const Message<T>& msg)
			{
				if (IsConnected())
					m_connection->Send(msg);
			}

		private:
			//asio context handles the data transfer
			asio::io_context m_context;
			// but needs a thread of its own to execute its work commands
			std::thread thrContext;
			//This is the hardware socket that is connected to the server
			asio::ip::tcp::socket m_socket;
			//The client has a single instance of a "Connection" object, which handles data transfer
			std::unique_ptr<Connection<T>> m_connection;
			//Thread safe queue of incoming messages from server
			TSQueue<OwnedMessage<T>> m_qMessagesIn;

		};
	}
