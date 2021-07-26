#include "catch2/catch.hpp"
#include "common/hal/io.hpp"
#include "pipettes/core/pipette_messages.hpp"
#include "pipettes/core/uart_message_handler.hpp"
#include "pipettes/tests/test_motor_control.hpp"

using namespace test_motor_control;

template <typename T, std::size_t L>
struct DataWriter {
    void write(std::span<uint8_t>& p) {
        for (auto i : p) {
            *iter++ = i;
        }
    }

    std::array<T, L> buff{};
    typename std::array<T, L>::iterator iter = buff.begin();
};

SCENARIO("messages can control motor") {
    GIVEN("stop command") {
        communication::MessageWriter message_writer{};
        TestMotorControl motor{};
        DataWriter<uint8_t, 8> dw{};
        MessageHandler handler{dw, motor, message_writer};
        pipette_messages::ReceivedMessage message;
        message = pipette_messages::Stop{};
        handler.handle_message(message);

        WHEN("the message is handled") {
            THEN("the state of the motor should be stopped") {
                REQUIRE(motor.state == "stopped");
            }
        }
    }

    GIVEN("move command") {
        communication::MessageWriter message_writer{};
        TestMotorControl motor{};
        DataWriter<uint8_t, 8> dw{};
        MessageHandler handler{dw, motor, message_writer};
        pipette_messages::ReceivedMessage message;
        message = pipette_messages::Move{};
        handler.handle_message(message);

        WHEN("the message is handled") {
            THEN("the state of the motor is now moved") {
                REQUIRE(motor.state == "moved");
            }
        }
    }

    GIVEN("status command") {
        communication::MessageWriter message_writer{};
        TestMotorControl motor{};
        DataWriter<uint8_t, 8> dw{};
        MessageHandler handler{dw, motor, message_writer};
        pipette_messages::ReceivedMessage message;
        message = pipette_messages::Status{};
        handler.handle_message(message);

        WHEN("the message is handled") {
            THEN("the writer should write the motor status and data") {
                REQUIRE(motor.state == "got_status");
                pipette_messages::GetStatusResult m{0x00, 0x00};
                DataWriter<uint8_t, 8> w{};
                message_writer.write(w, m);
                REQUIRE(dw.buff == w.buff);
            }
        }
    }

    GIVEN("setup command") {
        communication::MessageWriter message_writer{};
        TestMotorControl motor{};
        DataWriter<uint8_t, 8> dw{};
        MessageHandler handler{dw, motor, message_writer};
        pipette_messages::ReceivedMessage message;
        message = pipette_messages::Setup{};
        handler.handle_message(message);

        WHEN("the message is handled") {
            THEN("the state of the motor should be updated") {
                REQUIRE(motor.state == "setup");
            }
        }
    }

    GIVEN("get speed command") {
        communication::MessageWriter message_writer{};
        TestMotorControl motor{};
        DataWriter<uint8_t, 8> dw{};
        MessageHandler handler{dw, motor, message_writer};
        pipette_messages::ReceivedMessage message;
        message = pipette_messages::GetSpeed{};
        handler.handle_message(message);

        WHEN("the message is handled") {
            THEN("the writer should write the speed result") {
                pipette_messages::GetSpeedResult m{0x00};
                DataWriter<uint8_t, 8> w{};
                message_writer.write(w, m);
                REQUIRE(dw.buff == w.buff);
            }
        }
    }

    GIVEN("set speed command") {
        communication::MessageWriter message_writer{};
        TestMotorControl motor{};
        DataWriter<uint8_t, 8> dw{};
        MessageHandler handler{dw, motor, message_writer};
        pipette_messages::ReceivedMessage message;
        message = pipette_messages::SetSpeed{0x10};
        handler.handle_message(message);

        WHEN("the message is handled") {
            THEN("the motor data should be updated") {
                REQUIRE(motor.get_current_data() == 0x10);
            }
        }
    }
}

SCENARIO("process_buffer works") {
    GIVEN("a 5 byte buffer") {
        auto rxBuffer = test_motor_control::BufferType{1, 2, 3, 4, 5};
        uint8_t status = 0x0;
        uint32_t data = 0x0;
        TestMotorControl::process_buffer(rxBuffer, status, data);
        WHEN("process_buffer is called") {
            THEN("the status and data are updated") {
                REQUIRE(status == 0x01);
                REQUIRE(data == 0x2030405);
            }
        }
    }
}