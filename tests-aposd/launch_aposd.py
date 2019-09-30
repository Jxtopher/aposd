import os
import signal
import subprocess
import logging



process = subprocess.Popen(["build/aposd-Release", "-c", "/mnt/c/Dev/GitHub/aposd/configuration/aposd-webApps.json"], stdout=subprocess.PIPE)
logging.debug("[+] Launch aposd in backgroud")


logging.debug("[+] Send end signals to aposd")
process.send_signal(signal.SIGTERM)