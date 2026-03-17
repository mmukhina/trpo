#!/usr/bin/env python3
import sys
import json

def main():
    # Check command line arguments
    if len(sys.argv) < 8:
        print("Error: Insufficient arguments", file=sys.stderr)
        sys.exit(1)
    
    # Get arguments
    search_text = sys.argv[1]
    c_pod = sys.argv[2] == "1"
    c_skaz = sys.argv[3] == "1"
    c_opred = sys.argv[4] == "1"
    c_dop = sys.argv[5] == "1"
    c_ob = sys.argv[6] == "1"
    c_none = sys.argv[7] == "1"
    
    # Print received arguments for debugging (optional)
    # print(f"Search text: {search_text}", file=sys.stderr)
    # print(f"Checkboxes: pod={c_pod}, skaz={c_skaz}, opred={c_opred}, dop={c_dop}, ob={c_ob}, none={c_none}", file=sys.stderr)
    
    # Your actual processing logic here
    # This is just an example - replace with your actual search logic
    results = []
    
    # Split the search text into words
    words = search_text.split()
    
    # Example processing: just return the words that match certain criteria
    # based on checkbox selections
    for word in words:
        # Your actual logic here
        results.append(word)
    
    # If you want to return JSON instead of plain text:
    # print(json.dumps(results))
    
    # For plain text output (one result per line)
    for result in results:
        print(result)
    
    # Exit successfully
    sys.exit(0)

if __name__ == "__main__":
    main()