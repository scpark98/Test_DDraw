
[구현 방식]
- 이미지 객체들을 로딩하고 필요할 때 Invalidate()을 통해 OnPaint()에서 render
- render thread를 구동하고 flag에 따라 re render

[Direct2D] Device context 사용하기 (ID2D1DeviceContext)
https://milleatelier.tistory.com/156
이전 글 까지 Direct2D의 렌더링에 Render target을 사용했습니다.

[Direct2D] 비트맵 이미지 그리기
정확히는 ID2D1HwndRenderTarget객체를 사용했습니다.
이 객체는 ID2D1RenderTarget의 하위 객체입니다.
Microsoft는 Windows8발매 이후, 새 렌더링 API를 소개했습니다.
...


