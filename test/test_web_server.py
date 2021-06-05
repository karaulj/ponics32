import unittest
import threading
import requests


@unittest.skip
class TestHomePageHandler(unittest.TestCase):

    def setUp(self):
        #print("#################\nStarting ESP32Ponics Home Page tests\n#################")
        self.url_base = "http://192.168.254.25"
        self.homePath = '/'
        self.concurrentPass = 1
        pass

    def tearDown(self):
        #print("#################\nFinished ESP32Ponics Home Page tests\n#################")
        pass

    def sendReq(self, path):
        requests.get(self.url_base+path)

    def thrExcHandler(self, args):
        self.concurrentPass = 0

    #@unittest.skip
    def test_status_code_and_content_type_correct(self):
        r = requests.get(self.url_base+self.homePath)
        self.assertEqual(200, r.status_code)
        self.assertEqual("text/html", r.headers["Content-Type"])

    #@unittest.skip
    def test_concurrent_requests(self):
        threading.excepthook = self.thrExcHandler
        all_threads = [threading.Thread(target=self.sendReq(self.homePath)) for i in range(100)]
        for thr in all_threads:
            thr.start()
        for thr in all_threads:
            thr.join()
        self.assertEqual(1, self.concurrentPass)
        print("Finished concurrent requests test")


#@unittest.skip
class TestAPIHandler(unittest.TestCase):

    def setUp(self):
        #print("#################\nStarting ESP32Ponics API tests\n#################")
        self.url_base = "http://192.168.254.25"
        self.sensorReadingPath = '/getReading?id={id}&'
        self.actuatorDrivePath = '/driveActuator?id={id}&driveVal={dv}&'
        self.concurrentPass = 1
        pass

    def tearDown(self):
        #print("#################\nFinished ESP32Ponics API tests\n#################")
        pass

    def sendGETReq(self, path):
        requests.get(self.url_base + path)
    def sendPOSTReq(self, path):
        requests.post(self.url_base + path)

    def thrExcHandler(self, args):
        self.concurrentPass = 0

    #@unittest.skip
    def test_sensor_reading_status_code_and_content_type_correct(self):
        req_url = self.url_base + self.sensorReadingPath.format(id=0)
        print("req url: {}".format(req_url))
        r = requests.get(req_url)   # tests sensor w/ id 0 by default
        self.assertEqual(200, r.status_code)
        self.assertEqual("text/plain", r.headers["Content-Type"])

    #@unittest.skip
    def test_actuator_drive_status_code_and_content_type_correct(self):
        req_url = self.url_base + self.actuatorDrivePath.format(id=0, dv=0)
        print("req url: {}".format(req_url))
        r = requests.post(req_url)  # tests sensor w/ id 0 by default
        self.assertEqual(200, r.status_code)
        self.assertEqual("text/plain", r.headers["Content-Type"])

    #@unittest.skip
    def test_sensor_reading_concurrent_requests(self):
        threading.excepthook = self.thrExcHandler
        all_threads = [threading.Thread(target=self.sendGETReq(self.sensorReadingPath.format(id=0))) for i in range(100)]
        for thr in all_threads:
            thr.start()
        for thr in all_threads:
            thr.join()
        self.assertEqual(1, self.concurrentPass)
        print("Finished concurrent requests test")


    #@unittest.skip
    def test_actuator_drive_concurrent_requests(self):
        threading.excepthook = self.thrExcHandler
        all_threads = [threading.Thread(target=self.sendPOSTReq(self.actuatorDrivePath.format(id=0, dv=0))) for i in range(100)]
        for thr in all_threads:
            thr.start()
        for thr in all_threads:
            thr.join()
        self.assertEqual(1, self.concurrentPass)
        print("Finished concurrent requests test")


if __name__ == "__main__":
    print("#################\nStarting ESP32Ponics Web Server tests\n#################")
    unittest.main(verbosity=2)
    print("#################\nFinished ESP32Ponics Web Server tests\n#################")
