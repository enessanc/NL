#pragma once
#include "Common.h"


	namespace NL
	{
		//Message header is sent at start of all messages. The template allows us
		//to use "enum class" to ensure that the messages are valid at compile time
		template<typename T>
		struct MessageHeader
		{
			T id{};
			uint32_t size = 0;
		};

		template<typename T>
		struct Message
		{
			MessageHeader<T> header{};
			std::vector<uint8_t> body;

			//returns size of entire Message packet in bytes

			size_t size() const
			{
				return body.size();
			}

			//Overriding for std::cout compatibility - produces frinedly description of Message
			friend std::ostream& operator<<(std::ostream& os, const Message<T>& msg)
			{
				os << "ID:" << int(msg.header.id) << "Size:" << msg.header.size;
				return os;
			}

			//Pushes any POD-like data into the Message buffer
			template <typename DataType>
			friend Message<T>& operator<<(Message<T>& msg, const DataType& data)
			{
				//Check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to serialize in sending");

				//Cache current size of vector, as this will be point we insert the data
				size_t i = msg.body.size();

				//Resize the vector by size of the data being pushed
				msg.body.resize(msg.body.size() + sizeof(DataType));

				//Physically copy the data into newly allocated vector space
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

				//Recalculate the Message size
				msg.header.size = msg.size();

				//Return target msg, so it can be chained
				return msg;
			}

			friend Message<T>& operator<<(Message<T>& msg, const std::string& data)
			{
				//Cache current size of vector, as this will be point we insert the data
				size_t i = msg.body.size();

				//Resize the vector by size of the data being pushed
				msg.body.resize(msg.body.size() + data.size());

				//Physically copy the data into newly allocated vector space
				std::memcpy(msg.body.data() + i, data.data(), data.size());

				//Recalculate the Message size
				msg.header.size = msg.size();

				//Return target msg, so it can be chained
				return msg;
			}


			//Reading data via msg
			template <typename DataType>
			friend Message<T>& operator>>(Message<T>& msg, DataType& data)
			{
				//Check the type of data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to serialize in getting");

				//Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - sizeof(DataType);

				// Physically copy the data from the vector into user variable
				std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

				//Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);

				//Recalculate the msg size
				msg.header.size = msg.size();

				//Return the target Message so it can be chained
				return msg;
			}


			friend void operator>>(Message<T>& msg, std::string& data)
			{
				data.assign(msg.body.begin(), msg.body.end());

				//Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(0);

				//Recalculate the msg size
				msg.header.size = msg.size();

			}

		};

		template<typename T>
		class Connection;

		template<typename T>
		struct OwnedMessage
		{
			std::shared_ptr<Connection<T>> remote = nullptr;
			Message<T> msg;

			friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg)
			{
				os << msg.msg;
				return os;
			}
		};
    }
