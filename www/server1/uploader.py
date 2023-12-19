import os
import sys
from datetime import datetime

def printError(error_str):
    print "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html;charset=utf-8\r\n\r\n",
    print "<H1>" + error_str + "</H1>"


# Get the file path
file_path = os.environ.get("BODY_FILE")
file_to_upload = os.environ.get("PATH_INFO")
file_name = "new_file"
file_type = "jpeg"


# Check if the file exists
if os.path.exists(file_path):
    try:
        # Open the file for reading
        with open(file_path, "r") as file:
            # Read the content of the file
            file_data = file.read()

        if file_data:
            # Split the content into lines
            lines = file_data.split("\n")
            elements = lines[1].split('=')
            last_element = elements[-1]
            file_name = last_element[1 : -2]
            # Remove the first 3 lines and the last line
            # if file_name is empty then print error
            if file_name == "":
                    printError("File name is empty.")
                    sys.exit(0)
            if len(lines) >= 4:
                lines = lines[4:-2]

                # Join the remaining lines back into a single string
                file_data = "\n".join(lines)

                # Get the current time
                current_time = datetime.now()

                # Format the current time as a string with a specific format
                time_format = "%Y%m%d%H%M%S"  # You can customize the format as needed
                formatted_time = current_time.strftime(time_format)

                # Create a new file name with the current time at the beginning
                new_file_name = "{}_{}".format(formatted_time, file_name)

                # Get the path where you want to create the new file
                new_file_path = file_to_upload

                # Check if PATH_TRANSLATED is set
                if new_file_path:
                    new_file_path = os.path.join(new_file_path, new_file_name)

                    try:
                        # Create and open the new file for writing
                        with open(new_file_path, "w") as new_file:
                            # Write response and 200 status in header to the new file
                            print("HTTP/1.1 200 OK\r\n\r\n")

                            # Write the modified content to the new file
                            new_file.write(file_data)

                        print("Data written to the new file: {}".format(new_file_path))
                        # finish the response
                        print("\r\n\r\n")
                    except Exception as e:
                        printError("Error creating the new file: {}".format(e))

                else:
                    printError("PATH_TRANSLATED environment variable is not set.")
            else:
                printError("File does not contain enough lines to remove.")
        else:
            printError("File is empty.")
    except Exception as e:
        printError("Error: {}".format(e))

else:
    printError("File not found at:: {}".format(file_path))
