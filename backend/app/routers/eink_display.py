from fastapi import APIRouter, HTTPException
from fastapi.responses import PlainTextResponse
from pydantic import BaseModel, Field

router = APIRouter()


class EinkDisplayTextUpdate(BaseModel):
    text: str = Field(..., min_length=1, max_length=120)


class EinkDisplayTextResponse(BaseModel):
    text: str


_display_state = {"text": "The Embedded alliance"}


@router.get("/eink-display", response_model=EinkDisplayTextResponse)
def get_eink_display_text_json():
    return EinkDisplayTextResponse(text=_display_state["text"])


@router.get("/eink-display/text", response_class=PlainTextResponse)
def get_eink_display_text_plain():
    return _display_state["text"]


@router.put("/eink-display", response_model=EinkDisplayTextResponse)
def set_eink_display_text(payload: EinkDisplayTextUpdate):
    text = payload.text.strip()
    if not text:
        raise HTTPException(status_code=422, detail="Text cannot be empty")

    _display_state["text"] = text
    return EinkDisplayTextResponse(text=text)
