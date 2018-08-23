# coding=utf-8
import unittest
import os

class CustomTest(unittest.TestCase):
    
    def test_init(self):
        print("first test")
        self.assertTrue(True)

    def test_os_system(self):
        os.system("E:\Projects\RONew\clientcode\ToolsProject\LocalServer\Servers\start_all.bat")

if __name__ == '__main__':
    unittest.main()