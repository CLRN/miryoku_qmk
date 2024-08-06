import hid
import random
import datetime
import time

vendor_id = 0x4653
product_id = 0x0001

usage_page = 0xFF60
usage = 0x61
report_length = 32


def get_raw_hid_interface_path():
    device_interfaces = hid.enumerate(vendor_id, product_id)
    raw_hid_interfaces = [i for i in device_interfaces if i["usage_page"] == usage_page and i["usage"] == usage]

    if len(raw_hid_interfaces) == 0:
        raise Exception(f"unable to find kb in {device_interfaces}")

    return raw_hid_interfaces[0]["path"]


def send_raw_report(data: str | list[int]):
    with hid.Device(path=get_raw_hid_interface_path()) as device:
        request_data = [0x00] * (report_length + 1)  # First byte is Report ID
        if isinstance(data, str):
            request_data[1 : len(data) + 1] = list(map(ord, data))
        else:
            request_data[1 : len(data) + 1] = data

        request_report = bytes(request_data)

        device.write(request_report)


if __name__ == "__main__":
    # send_raw_report(f"shello left side\nok?\n{datetime.datetime.now()}\0")
    # send_raw_report(" hello right side\nhaha\0")
    while True:
        send_raw_report(f"\0{datetime.datetime.now().isoformat()}")
        send_raw_report(f"\2{datetime.datetime.now().isoformat()}")
        send_raw_report([1, random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)])
        time.sleep(1)
