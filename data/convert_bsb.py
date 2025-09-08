#!/usr/bin/env python3

import re
import sys

# Book abbreviations mapping (KJV style abbreviations)
book_abbr = {
    "Genesis": "Ge",
    "Exodus": "Ex",
    "Leviticus": "Le",
    "Numbers": "Nu",
    "Deuteronomy": "De",
    "Joshua": "Jos",
    "Judges": "Jg",
    "Ruth": "Ru",
    "1 Samuel": "1Sa",
    "2 Samuel": "2Sa",
    "1 Kings": "1Ki",
    "2 Kings": "2Ki",
    "1 Chronicles": "1Ch",
    "2 Chronicles": "2Ch",
    "Ezra": "Ezr",
    "Nehemiah": "Ne",
    "Esther": "Es",
    "Job": "Job",
    "Psalms": "Ps",  # KJV uses plural
    "Proverbs": "Pr",
    "Ecclesiastes": "Ec",
    "Song of Solomon": "So",
    "Isaiah": "Isa",
    "Jeremiah": "Jer",
    "Lamentations": "La",
    "Ezekiel": "Eze",
    "Daniel": "Da",
    "Hosea": "Ho",
    "Joel": "Joe",
    "Amos": "Am",
    "Obadiah": "Ob",
    "Jonah": "Jon",
    "Micah": "Mic",
    "Nahum": "Na",
    "Habakkuk": "Hab",
    "Zephaniah": "Zep",
    "Haggai": "Hag",
    "Zechariah": "Zec",
    "Malachi": "Mal",
    "Matthew": "Mt",
    "Mark": "Mk",
    "Luke": "Lk",
    "John": "Jn",
    "The Acts": "Ac",  # KJV uses "The Acts"
    "Acts": "Ac",      # BSB uses "Acts"
    "Romans": "Ro",
    "1 Corinthians": "1Co",
    "2 Corinthians": "2Co",
    "Galatians": "Ga",
    "Ephesians": "Eph",
    "Philippians": "Php",
    "Colossians": "Col",
    "1 Thessalonians": "1Th",
    "2 Thessalonians": "2Th",
    "1 Timothy": "1Ti",
    "2 Timothy": "2Ti",
    "Titus": "Tit",
    "Philemon": "Phm",
    "Hebrews": "Heb",
    "James": "Jas",
    "1 Peter": "1Pe",
    "2 Peter": "2Pe",
    "1 John": "1Jn",
    "2 John": "2Jn",
    "3 John": "3Jn",
    "Jude": "Jud",
    "Revelation": "Rev"
}

# Book order (to assign numbers) - using KJV book names
book_order = [
    "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy",
    "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel",
    "1 Kings", "2 Kings", "1 Chronicles", "2 Chronicles",
    "Ezra", "Nehemiah", "Esther", "Job", "Psalms",
    "Proverbs", "Ecclesiastes", "Song of Solomon",
    "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel",
    "Hosea", "Joel", "Amos", "Obadiah", "Jonah",
    "Micah", "Nahum", "Habakkuk", "Zephaniah", "Haggai",
    "Zechariah", "Malachi",
    "Matthew", "Mark", "Luke", "John",
    "The Acts",  # KJV uses "The Acts"
    "Romans", "1 Corinthians", "2 Corinthians",
    "Galatians", "Ephesians", "Philippians", "Colossians",
    "1 Thessalonians", "2 Thessalonians",
    "1 Timothy", "2 Timothy", "Titus", "Philemon",
    "Hebrews", "James", "1 Peter", "2 Peter",
    "1 John", "2 John", "3 John", "Jude", "Revelation"
]

def parse_bsb_line(line):
    """Parse a line from the BSB text file and return components"""
    # Skip header lines
    if line.startswith("The Holy Bible") or line.startswith("This text") or line.startswith("Verse"):
        return None
    
    # Match the format "Book Chapter:Verse\tText"
    match = re.match(r'^([1-3]?\s?[A-Za-z]+(?:\s[A-Za-z]+)*)\s+(\d+):(\d+)\t(.*)$', line)
    if not match:
        return None
    
    book = match.group(1)
    chapter = int(match.group(2))
    verse = int(match.group(3))
    text = match.group(4)
    
    # Normalize book name
    book = book.strip()
    
    # Handle special cases
    if book == "Psalm":
        book = "Psalms"  # Convert to KJV format
    elif book == "Acts":
        book = "The Acts"  # Convert to KJV format
    
    return book, chapter, verse, text

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input_bsb.txt> <output_bsb.tsv>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    with open(input_file, 'r', encoding='utf-8') as infile, \
         open(output_file, 'w', encoding='utf-8') as outfile:
        
        for line in infile:
            result = parse_bsb_line(line.strip())
            if result is None:
                continue
                
            book, chapter, verse, text = result
            
            # Get book abbreviation and number
            if book not in book_abbr:
                print(f"Warning: Unknown book '{book}'", file=sys.stderr)
                continue
                
            abbr = book_abbr[book]
            
            # Find book number in order
            try:
                book_num = book_order.index(book) + 1
            except ValueError:
                print(f"Warning: Book '{book}' not in standard order", file=sys.stderr)
                continue
            
            # Write in TSV format: Book Name, Abbreviation, Book Number, Chapter, Verse, Text
            outfile.write(f"{book}\t{abbr}\t{book_num}\t{chapter}\t{verse}\t{text}\n")

if __name__ == "__main__":
    main()