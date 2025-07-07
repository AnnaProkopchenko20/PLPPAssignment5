import subprocess

def run_echo(input_data):
    process = subprocess.Popen(
        ['main.exe'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()


def test_echo():
    tests = [
        {
            "input": "min(1,2)\n",
            "expected_output": "1"
        },
        {
            "input": "1*2\n",
            "expected_output": "2"
        },
        {
            "input": "3-4\n",
            "expected_output": "-1"
        },
        {
            "input": "2*3*4\n",
            "expected_output": "24"
        },
        {
            "input": "10+5-3\n",
            "expected_output": "12"
        },
        {
            "input": "20/2/2\n",
            "expected_output": "5"
        },
        {
            "input": "min(min(1,2),3)\n",
            "expected_output": "1"
        },
        {
            "input": "min(min(1,2),min(1,2))\n",
            "expected_output": "1"
        },
        {
            "input": "max(max(1,2),3)\n",
            "expected_output": "3"
        },
        {
            "input": "min(min(1234,5678),min(2345,6789)) * 50 - 2300 / 100\n",
            "expected_output": "61677"
        },
        {
            "input": "max(max(8500,4200),max(7100,9800)) + 300 * 25\n",
            "expected_output": "17300"
        },
        {
            "input": "min( min( 1234 , 5678 ) , min( 2345 , 6789 ) ) * 50 - 2300 / 100\n",
            "expected_output": "61677"
        },
        {
            "input": "min(1,max(2,3))\n",
            "expected_output": "1"
        },
        {
            "input": "max(1,min(2,3))\n",
            "expected_output": "2"
        },
        {
            "input": "   5   +   3   \n",
            "expected_output": "8"
        },

        {
            "input": "max(max(8500,4200),max(7100,9800)) + 300 * 25\n",
            "expected_output": "17300"
        },
        {
            "input": "(-1) * ((-2) - (-3))\n",
            "expected_output": "-1"
        },
        {
            "input": "min( min( 1234 , 5678 ) , min( 2345 , 6789 ) ) * 50 - 2300 / 100\n",
            "expected_output": "61677"
        },
        {
            "input": "abs(-450) + min(100, 200) * max(2, 3)\n",
            "expected_output": "750"
        },
        {
            "input": "pow(2, 3) + pow(5, 2) - abs(-12)\n",
            "expected_output": "21"
        },
        {
            "input": "min(abs(-789), max(456, 123)) + 100\n",
            "expected_output": "556"
        },
        {
            "input": "(max(15, 25) + min(30, 40)) * pow(2, 2)\n",
            "expected_output": "220"
        },
        {
            "input": "abs(min(-50, -100)) / max(5, 10) + 300\n",
            "expected_output": "310"
        },
        {
            "input": "pow(max(2, 3), min(4, 5)) - abs(-200) + 150\n",
            "expected_output": "31"
        },
        {
            "input": "min(1000, max(500, 800)) + abs(-75) * 2\n",
            "expected_output": "950"
        },
        {
            "input": "(abs(-25) + max(10, 15)) * min(3, 4) / pow(2, 2)\n",
            "expected_output": "30"
        },
        {
            "input": "max(pow(3, 2), abs(-8)) + min(100, 200) - 50\n",
            "expected_output": "59"
        },
        {
            "input": "min(max(1500, 2000), abs(-1800)) + 250 / 5\n",
            "expected_output": "1850"
        }
    ]

    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]

        output, error = run_echo(input_data)

        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output.strip() != expected_output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")


if __name__ == "__main__":
    test_echo()