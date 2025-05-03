import os
import uvicorn

from fastapi import FastAPI
from pydantic import BaseModel
from dotenv import load_dotenv
from groq import Groq

# FastAPI entrypoint
app = FastAPI()

# Load .env file data (Groq Api Key)
load_dotenv()

# Groq client instance (created with loaded Api Key)
client = Groq(api_key = os.getenv('GROQ_API_KEY'))

# Message class
class Message(BaseModel):
    role: str
    content: str

# Request Data class
class RequestData(BaseModel):
    messages: list[Message]

# Inner function invoking chosen language model
def generate_response(messages):
    chat_completion = client.chat.completions.create(
        messages=[message.dict() for message in messages],
        model="llama3-70b-8192",
    )

    return chat_completion.choices[0].message.content

# Proper function to send model response to the game
@app.post("/generate/")
async def generate_response_endpoint(data: RequestData):
    response = generate_response(data.messages)
    print("User prompt received: ", data.messages[-1].content)
    print("Answering with: ", response)

    return response

# Start local server at program start up
if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
