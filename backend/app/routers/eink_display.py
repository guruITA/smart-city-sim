from typing import Literal

from fastapi import APIRouter, HTTPException
from fastapi.responses import PlainTextResponse
from pydantic import BaseModel, Field

router = APIRouter()


class EinkDisplayTextUpdate(BaseModel):
    text: str = Field(..., min_length=1, max_length=120)


class EinkDisplayStateUpdate(BaseModel):
    text: str = Field(..., min_length=1, max_length=120)
    text_color: Literal["black", "red", "white"] = Field(default="black")
    image_enabled: bool = Field(default=False)
    image_color: Literal["black", "red", "white"] = Field(default="red")
    image_width: int = Field(default=0, ge=0, le=200)
    image_height: int = Field(default=0, ge=0, le=200)
    image_rows: list[str] = Field(default_factory=list)


class EinkDisplayTextResponse(BaseModel):
    text: str


class EinkDisplayStateResponse(BaseModel):
    text: str
    text_color: str
    image_enabled: bool
    image_color: str
    image_width: int
    image_height: int
    image_rows: list[str]


_display_state = {
    "text": "The Embedded alliance",
    "text_color": "black",
    "image_enabled": True,
    "image_color": "red",
    "image_width": 16,
    "image_height": 16,
    "image_rows": [
        "0000000011110000",
        "0000001111111100",
        "0000111100001111",
        "0001110000000111",
        "0011100000000011",
        "0111001111110011",
        "0110011000011001",
        "1110010000001001",
        "1110010011001001",
        "1110010000001001",
        "0110011000011001",
        "0111001111110011",
        "0011100000000011",
        "0001110000000111",
        "0000111100001111",
        "0000001111111100",
    ],
}


def _build_payload() -> str:
    rows = ";".join(_display_state["image_rows"])
    return "\n".join(
        [
            f'TEXT={_display_state["text"]}',
            f'TEXT_COLOR={_display_state["text_color"]}',
            f'IMAGE_ENABLED={1 if _display_state["image_enabled"] else 0}',
            f'IMAGE_COLOR={_display_state["image_color"]}',
            f'IMAGE_WIDTH={_display_state["image_width"]}',
            f'IMAGE_HEIGHT={_display_state["image_height"]}',
            f'IMAGE_ROWS={rows}',
        ]
    )


def _validate_image_payload(image_enabled: bool, image_width: int, image_height: int, image_rows: list[str]) -> None:
    if not image_enabled:
        return

    if image_width <= 0 or image_height <= 0:
        raise HTTPException(status_code=422, detail="Image width and height must be greater than zero when image_enabled is true")

    if len(image_rows) != image_height:
        raise HTTPException(status_code=422, detail="image_rows must contain exactly image_height rows")

    for row in image_rows:
        if len(row) != image_width:
            raise HTTPException(status_code=422, detail="Each image row must match image_width")
        if any(pixel not in {"0", "1"} for pixel in row):
            raise HTTPException(status_code=422, detail="Image rows may only contain 0 and 1 characters")


@router.get("/eink-display", response_model=EinkDisplayStateResponse)
def get_eink_display_text_json():
    return EinkDisplayStateResponse(**_display_state)


@router.get("/eink-display/text", response_class=PlainTextResponse)
def get_eink_display_text_plain():
    return _display_state["text"]


@router.get("/eink-display/payload", response_class=PlainTextResponse)
def get_eink_display_payload_plain():
    return _build_payload()


@router.put("/eink-display", response_model=EinkDisplayStateResponse)
def set_eink_display_text(payload: EinkDisplayStateUpdate):
    text = payload.text.strip()
    if not text:
        raise HTTPException(status_code=422, detail="Text cannot be empty")

    _validate_image_payload(payload.image_enabled, payload.image_width, payload.image_height, payload.image_rows)

    _display_state["text"] = text
    _display_state["text_color"] = payload.text_color
    _display_state["image_enabled"] = payload.image_enabled
    _display_state["image_color"] = payload.image_color
    _display_state["image_width"] = payload.image_width
    _display_state["image_height"] = payload.image_height
    _display_state["image_rows"] = payload.image_rows
    return EinkDisplayStateResponse(**_display_state)
