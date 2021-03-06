import argparse
import json
import logging
import os
import requests
import signal
import subprocess
import sys

from typing import Dict


class AposdClient:
    def __init__(self, is_development: bool):
        self.url = "http://localhost:8080" if is_development else "https://aposd.limerick.fr"

    def send(self, method: str, msg: Dict) -> Dict:
        # Example echo method
        payload = {
            "method": method,
            "params": [json.dumps(msg)],  # NOTE (vivi): why use dumps here?
            "id": 0,  # NOTE (vivi): why always 0?
        }
        print("[Send] ", payload)
        return requests.post(self.url, json=payload).json()

    def echo_test(self):
        response = self.send("echo", {"message": "echome!"})
        print(response)
        return response


class Aposd:
    def __init__(self, aposd_client: AposdClient):
        self.aposd_client = aposd_client
        self.object_id = -1

    def initialization(self):
        aposd = {
            "seed": 0,
            "Interface": "WEBAPPLICATION",
            "CalculationModel": {
                "ParameterSelection": {
                    "className": "PsAdaptivePursuit", # learning method AP
                    "alpha" : 0.2,
                    "beta" : 0.2,
                    "p_min" : 0.1,
                    "p_max" : 0.9,
                    "aggregation_function":"max",
                    "heterogeneity_policy":"heterogenous",
                    "number_of_parameters": 2,
                },
                "SolutionSelection" : "max"
            },
            "initial_solution": {"fitness": [0], "fitnessIsValid": [1], "solution": ["empty"]},
        }
        recv = self.aposd_client.send("initialization", aposd)
        if 'error' in recv:
            print("Error initialization")
            exit(0)
        else :
            self.object_id = recv["object_id"]
            return recv

    def learning(self, fitness_t0 : int, fitness_t1 : int, num_paramter: int):
        aposd = {
            "object_id": self.object_id,
            "Solution_t0" : {
                "fitness" : [fitness_t0],
                "fitnessIsValid" : [1],
                "solution" : ["empty"]
            },
            "Solution_t1" : {
                "fitness" : [fitness_t1],
                "fitnessIsValid" : [1],
                "solution" : ["empty"]
            },
            "num_paramter": num_paramter,
        }
        return self.aposd_client.send("learning", aposd)

    def finish(self):
        aposd = {"object_id": self.object_id}
        return self.aposd_client.send("finish", aposd)


def main(is_development: bool, is_echo_test: bool):
    signal.signal(signal.SIGINT, lambda sig, frame: sys.exit())

    aposd_client = AposdClient(is_development)
    if is_echo_test:
        aposd_client.echo_test()
        return

    cout_parameter_0 = 0
    cout_parameter_1 = 0

    a = Aposd(aposd_client)
    recv = a.initialization()
    print("[Recv]", recv)

    NUM_ITERATION = 20

    for i in range(NUM_ITERATION):
        if recv["num_paramter"] == 0:
            new_fitness = recv["Solution"]["fitness"][0] + 5  # NOTE (vivi): what's that "5"?
            cout_parameter_0 += 1

        elif recv["num_paramter"] == 1:
            new_fitness = recv["Solution"]["fitness"][0] + 1  # NOTE (vivi): what's that "1"?
            cout_parameter_1 += 1

        recv = a.learning(recv["Solution"]["fitness"][0], new_fitness, recv["num_paramter"])
        print("[Recv]", i, recv)

    recv = a.finish()
    print("[Recv]", NUM_ITERATION, recv)

    print("Parameter 0", cout_parameter_0)
    print("Parameter 1", cout_parameter_1)

    assert cout_parameter_1 < cout_parameter_0


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('path', type=str,  help='Path stocos executable')
    args = parser.parse_args()

    process = subprocess.Popen([args.path, "-c", "configuration/aposd-webApps.json"], stdout=subprocess.PIPE)
    logging.debug("[+] Launch aposd in backgroud")

    is_echo_test = False
    is_development = True
    main(is_development, is_echo_test)

    logging.debug("[+] Send end signals to aposd")
    process.send_signal(signal.SIGTERM)
    exit(0)
