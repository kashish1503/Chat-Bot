# Corpus Q&A Tool

The **Corpus Q&A Tool** is a system designed to answer complex natural language questions by retrieving and evaluating the most relevant passages from a large corpus of text, and optionally querying a language model such as ChatGPT to generate final answers.

## Overview

Given a large textual corpus (such as books or speeches), this tool helps locate the most relevant paragraphs for a user-provided question. It uses a combination of information retrieval techniques and large language models to generate precise and contextually rich answers.

This tool is particularly suited for research, historical analysis, and knowledge extraction from extensive documents.

## Features

- **Top-k Paragraph Retrieval**: Uses custom scoring based on corpus-specific and general word frequencies to rank and retrieve the most relevant paragraphs for a given query.
- **TF-IDF Based Scoring**: Adjusts relevance by comparing term frequency in the target corpus against a reference general corpus.
- **Linked List Representation**: Stores and returns top-k paragraphs using a linked list format.
- **LLM Integration**: Supports querying with large language models (e.g., ChatGPT) using the most relevant context.
- **Prompt Engineering Support**: Allows prompt customization to enhance query effectiveness and model response quality.

## How It Works

1. **Preprocessing**:
   - Input queries are tokenized and cleaned.
   - Stopwords may be ignored to focus on meaningful content terms.

2. **Scoring Words**:
   - Each word is scored as:
     ```
     score(w) = (frequency in target corpus + 1) / (frequency in general corpus + 1)
     ```

3. **Scoring Paragraphs**:
   - Paragraphs are scored based on the weighted sum of query word frequencies:
     ```
     score(p) = Σ [fp(w) * score(w)] for all query words w in paragraph p
     ```

4. **Top-k Ranking**:
   - Paragraphs with the highest scores are returned as the most relevant contexts.

5. **Querying the LLM**:
   - The top-k paragraphs are formatted into a prompt and optionally passed to an LLM API (e.g., ChatGPT).
   - Final answers are extracted based on the LLM’s output.

## Example Use Case

You can use the tool to answer questions such as:

> "What were the author's views on self-realization?"

The system will retrieve the top-k paragraphs related to the concept and optionally synthesize a response via an LLM.

## File Structure

- `qna_tool.cpp / .h` – Core logic for query processing and paragraph scoring.
- `dict.cpp / .h` – Dictionary and frequency mapping.
- `search.cpp / .h` – Functions to perform word-level corpus search.
- `api_call.py` – Handles interaction with ChatGPT or other LLMs.
- `Node.cpp / .h` – Data structures for storing paragraph metadata.

## Technologies Used

- C++ for core logic and performance
- Python for API interaction with language models
- Custom data structures (Linked Lists, Dictionaries)
- ChatGPT or open-source LLMs via APIs (optional)

## Getting Started

1. Preprocess the corpus and build frequency dictionaries.
2. Call `get_top_k_para(query, k)` to fetch the most relevant paragraphs.
3. Use `query(query_string, filename)` to generate and write the final response.
4. Optionally integrate with `query_llm` function for LLM-based answering.

## Optimization Tips

- Use stopword filtering to improve paragraph relevance.
- Adjust scoring weights to fine-tune result quality.
- Experiment with prompt phrasing to improve LLM output.
- Explore open-source LLMs to reduce dependency on paid APIs.
