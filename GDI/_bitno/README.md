# GDI - najbitniji prakticni koncepti i snippet-i koda

## Uvod

- `RGB(bRed, bGreen, bBlue)` - makro za definisanje boje tipa `COLORREF`;
- Korisne funkcije: 
  - `COLORREF CDC::GetTextColor()`, `COLORREF CDC::SetTextColor(COLORREF crColor)` - postavljanje boje teksta (**foreground color**);
- Korisni prefiksi - nema u ovom poglavlju;

### Moji komentari

- DC se treba posmatrati kao state masina - u sebi sadrzi odredjene atribute i graficke objekte, koji se mogu menjati po potrebi;
- Ukoliko se neki od atributa/grafickih objekata DC-a menja, potrebno je sacuvati prethodno stanje i po zavrsetku zadatka, vratiti DC u prethodno stanje;

- primer:

```c++
COLORREF purpleColor = RGB(127, 0, 255);

CBrush purpleDiagcrossBrush{ HS_DIAGCROSS, purpleColor };

CBrush* oldBrush = pDC->SelectObject(&purpleDiagcrossBrush);
CGdiObject* oldPen = pDC->SelectStockObject(NULL_PEN);

// ...

pDC->SelectObject(oldBrush);
pDC->SelectObject(oldPen);

purpleDiagcrossBrush.DeleteObject();
```

## Graficki objekti - olovke i cetke

- korisne funkcije:
  - `int CDC::GetBkMode()`, `int CDC::SetBkMode(int iBkMode)` (`TRANSPARENT` ili `OPAQUE`) - background mod;
  - `virtual CGdiObject* CDC::SelectStockObject(int nIndex)` - selektovanje gotovih GDI objekata (`NULL_PEN`, `BLACK_PEN`, `NULL_BRUSH`, ...);
- korisni makroi:;
- korisni prefiksi:
  - `PS_*` - definisanje **Pen Style**-a;
    - `PS_ENDCAP_*` - stil zavrsetka;
    - `PS_JOIN_*` - stil spoja;
  - `HS_*` - **Hatch Style** - za definisanje cetke sa srafurom;

### Olovke

- primer:

```c++
CPen gridPen{ PS_COSMETIC | PS_SOLID, 0, RGB(0, 0, 0) };
CPen boldLinesPen{ PS_GEOMETRIC | PS_SOLID, 5, RGB(0, 255, 255) };
```

## Crtanje primitiva - linije, mnogouglovi, krive

- korisne funkcije
  - `MoveTo`, `LineTo`, `Polyline`, `PolyPolyline` - iscrtavanje linija;
  - `Polygon`, `PolyPolygon`, `Rectangle` - mnogouglovi;
  - `Ellipse`, `RoundRect`;
  - `Arc` (`Get/SetArcDirection`, `AD_CLOCKWISE/COUNTERCLOCKWISE`), `Pie`, `Chord`;
  - `PolyBezier` - broj tacaka mora da bude obrasca $3n+1$, tako da je $n$ ceo broj;

## Fontovi

- korisne funkcije:
  - `BOOL CFont::CreateFont(...)` - od parametara su bitni **escapement**, **orientation** i **facename**;